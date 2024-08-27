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


namespace edp_calculation
{

// 层间位移角计算结果，包含楼层位移和层间位移角两组数据
class InterStoryDriftResult
{
    friend class FilteringIntegral;
    friend class ModifiedFilteringIntegral;

public:
    // 默认构造函数
    InterStoryDriftResult() = default;

    // 析构函数
    ~InterStoryDriftResult() = default;

    // 获取位移
    // @return 位移的引用
    data_structure::Displacement &get_displacement() { return displacement_; }
    // 获取层间位移角
    // @return 层间位移角的引用
    data_structure::InterStoryDrift &get_inter_story_drift()
    {
        return inter_story_drift_;
    }

private:
    // 计算结果，具体内容由方法决定
    data_structure::Displacement displacement_{};
    data_structure::InterStoryDrift inter_story_drift_{};
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
        : input_acceleration_(acceleration), building_(building)
    {}

    // 从加速度数据指针中构造
    // @param acceleration_ptr 加速度数据指针
    // @param building_ptr 建筑信息指针
    explicit BasicEdpCalculation(
        std::shared_ptr<const data_structure::Acceleration> acceleration_ptr,
        std::shared_ptr<data_structure::Building> building_ptr)
        : input_acceleration_(*acceleration_ptr), building_(*building_ptr)
    {}

    // 拷贝构造函数
    BasicEdpCalculation(const BasicEdpCalculation &other) = default;

    // 移动构造函数
    BasicEdpCalculation(BasicEdpCalculation &&other) = default;

    // 拷贝赋值函数
    BasicEdpCalculation &operator=(const BasicEdpCalculation &other) = default;

    // 移动赋值函数
    BasicEdpCalculation &operator=(BasicEdpCalculation &&other) = default;

    // 析构函数
    virtual ~BasicEdpCalculation() = default;

    // 从配置文件中读取参数
    virtual void LoadConfig(const std::string &config_file = "") = 0;

    // 计算工程需求参量函数入口
    virtual void CalculateEdp() = 0;

protected:
    // 单方向加速度数据指针
    data_structure::Acceleration input_acceleration_{};
    // 建筑信息的指针
    data_structure::Building building_{};
};

} // namespace edp_calculation

#endif // EDP_CALCULATION_BASIC_EDP_CALCULATION_H