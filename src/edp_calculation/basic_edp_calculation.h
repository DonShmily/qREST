/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\edp_calculation\basic_edp_calculation.h
** -----
** File Created: Thursday, 11th July 2024 23:53:41
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Saturday, 10th August 2024 22:26:30
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 工程需求参量计算基类。
// 每种方法的类都应当对应唯一的计算结果类，结果类和计算方法类定义在同个头文件中。

#ifndef EDP_CALCULATION_BASIC_EDP_CALCULATION_H
#define EDP_CALCULATION_BASIC_EDP_CALCULATION_H

// stdc++ headers
#include <memory>
#include <string>

// project headers
#include "data_structure/acceleration.h"
#include "data_structure/building.h"
#include "data_structure/displacement.h"
#include "data_structure/inter_story_drift.h"
#include "data_structure/velocity.h"
#include "data_structure/vibration.h"
#include "numerical_algorithm/interp.h"


namespace edp_calculation
{

// 层间位移角计算结果，包含测点和楼层的主要计算数据
class EdpResult
{
    friend class FilteringIntegral;
    friend class ModifiedFilteringIntegral;

public:
    // 默认构造函数
    EdpResult() = default;

    // 析构函数
    ~EdpResult() = default;

    // 获取各测点的计算结果
    // 获取各测点加速度
    // @return 测点加速度
    data_structure::Acceleration get_mea_acceleration()
    {
        return data_structure::Acceleration(
            measurement_avd_.acceleration.data_ptr(),
            measurement_avd_.frequency);
    }
    // 获取各测点速度
    // @return 测点速度
    data_structure::Velocity get_mea_velocity()
    {
        return data_structure::Velocity(measurement_avd_.velocity.data_ptr(),
                                        measurement_avd_.frequency);
    }
    // 获取各测点位移
    // @return 测点位移
    data_structure::Displacement get_mea_displacement()
    {
        return data_structure::Displacement(
            measurement_avd_.displacement.data_ptr(),
            measurement_avd_.frequency);
    }

    // 获取各楼层的计算结果
    // 获取各楼层加速度
    // @return 加速度
    data_structure::Acceleration get_acceleration()
    {
        if (!calculated_)
        {
            calculate_other();
        }
        return data_structure::Acceleration(avd_.acceleration.data_ptr(),
                                            avd_.frequency);
    }
    // 获取各楼层速度
    // @return 速度
    data_structure::Velocity get_velocity()
    {
        if (!calculated_)
        {
            calculate_other();
        }
        return data_structure::Velocity(avd_.velocity.data_ptr(),
                                        avd_.frequency);
    } // 获取各楼层位移
      // @return 位移
    data_structure::Displacement get_displacement()
    {
        return data_structure::Displacement(avd_.displacement.data_ptr(),
                                            avd_.frequency);
    }
    // 获取各楼层层间位移角
    // @return 层间位移角
    data_structure::InterStoryDrift get_inter_story_drift()
    {
        return inter_story_drift_;
    }

private:
    // 计算结果，具体内容由方法决定
    // 测点结果
    data_structure::Vibration measurement_avd_{};
    // 所有楼层的结果
    data_structure::Vibration avd_{};
    data_structure::InterStoryDrift inter_story_drift_{};
    // 各楼层的加速度和速度可能被重复计算，使用标志区分
    bool calculated_ = false;
    // 使用了何种插值方法
    numerical_algorithm::InterpType interp_type_ =
        numerical_algorithm::InterpType::CubicSpline;
    std::shared_ptr<data_structure::Building> building_;

    // 计算无测点层的速度和加速度
    void calculate_other()
    {
        calculated_ = true;
        // 插值计算无测点层的速度和加速度
        numerical_algorithm::Interp interp_function(interp_type_);
        avd_.velocity.data() =
            interp_function.Interpolation(building_->get_measuren_height(),
                                          measurement_avd_.velocity.data(),
                                          building_->get_floor_height());
        avd_.acceleration.data() =
            interp_function.Interpolation(building_->get_measuren_height(),
                                          measurement_avd_.acceleration.data(),
                                          building_->get_floor_height());
    }
};

// 工程需求参量计算基类，计算方法基类至少接收一个加速度数据
class BasicEdpCalculation
{
public:
    // 默认构造函数
    BasicEdpCalculation() = default;

    // 从加速度数据中构造
    // @param acceleration 加速度数据
    // @param building 建筑信息
    explicit BasicEdpCalculation(
        const data_structure::Acceleration &acceleration,
        const data_structure::Building &building)
        : input_acceleration_(acceleration),
          building_ptr_(std::make_shared<data_structure::Building>(building))
    {}

    // 从加速度数据指针中构造
    // @param acceleration_ptr 加速度数据指针
    // @param building_ptr 建筑信息指针
    explicit BasicEdpCalculation(
        std::shared_ptr<const data_structure::Acceleration> acceleration_ptr,
        std::shared_ptr<data_structure::Building> building_ptr)
        : input_acceleration_(*acceleration_ptr), building_ptr_(building_ptr)
    {}

    // 析构函数
    virtual ~BasicEdpCalculation() = default;

    // 从配置文件中读取参数
    virtual void LoadConfig(const std::string &config_file = "") = 0;

    // 计算工程需求参量函数入口
    virtual void CalculateEdp() = 0;

    // 当前输入是否已经完成计算
    bool is_calculated() const { return is_calculated_; }

protected:
    // 完成计算的标志
    bool is_calculated_ = false;
    // 单方向加速度数据
    data_structure::Acceleration input_acceleration_{};
    // 建筑信息
    std::shared_ptr<data_structure::Building> building_ptr_{};
};

} // namespace edp_calculation

#endif // EDP_CALCULATION_BASIC_EDP_CALCULATION_H