/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\parameters_identification\basic_parameters_identification.h
** -----
** File Created: Monday, 30th September 2024 15:02:42
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Monday, 30th September 2024 15:04:13
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:参数识别基类。

#ifndef PARAMETERS_IDENTIFICATION_BASIC_PARAMETERS_IDENTIFICATION_H
#define PARAMETERS_IDENTIFICATION_BASIC_PARAMETERS_IDENTIFICATION_H

// stdc++ headers
#include <vector>

// project headers
#include "data_structure/acceleration.h"
#include "data_structure/building.h"

// 参数识别命名空间
namespace parameters_identification
{

// 参数识别结果类
class PiResult
{
    friend class BasicParametersIdentification;
    friend class SimpleParametersIdentification;

public:
    // 默认构造函数
    PiResult() = default;

    // 析构函数
    ~PiResult() = default;

    // 获取识别的频率
    // @return 识别的频率
    std::vector<double> get_frequency() const { return frequency; }

    // 获取识别的周期
    // @return 识别的周期
    std::vector<double> get_period() const
    {
        std::vector<double> period;
        for (const auto &f : frequency)
        {
            period.push_back(1.0 / f);
        }
        return period;
    }

    // 获取识别的阻尼比
    // @return 识别的阻尼比
    std::vector<double> get_damping_ratio() const { return damping_ratio; }

    // 获取识别的振型向量
    // @return 识别的振型向量
    std::vector<std::vector<double>> get_mode_shape() const
    {
        return mode_shape;
    }

private:
    // 识别的频率
    std::vector<double> frequency;
    // 识别的阻尼比
    std::vector<double> damping_ratio;
    // 识别的振型向量
    std::vector<std::vector<double>> mode_shape;
}; // namespace class PiResult

class BasicParametersIdentification
{
public:
    // 默认构造函数
    BasicParametersIdentification() = default;

    // 从加速度数据和建筑信息指针构造
    // @param acceleration 单方向加速度数据
    // @param building 建筑信息指针
    BasicParametersIdentification(
        const data_structure::Acceleration &acceleration,
        const std::shared_ptr<data_structure::Building> &building)
        : input_acceleration_(acceleration), building_ptr_(building)
    {}

    // 从加速度数据和建筑信息构造
    // @param acceleration 单方向加速度数据
    // @param building 建筑信息
    BasicParametersIdentification(
        const data_structure::Acceleration &acceleration,
        const data_structure::Building &building)
        : input_acceleration_(acceleration),
          building_ptr_(std::make_shared<data_structure::Building>(building))
    {}

    // 析构函数
    virtual ~BasicParametersIdentification() = default;

    // 参数识别
    // @return 参数识别结果
    PiResult identify_parameters()
    {
        if (!is_calculated_)
        {
            Identify();
        }
        return pi_result_;
    }

    // 识别结构的固有周期和阻尼比
    virtual void Identify() = 0;

    // 是否已经完成计算
    // @return 是否已经完成计算
    bool is_calculated() const { return is_calculated_; }

protected:
    // 完成计算的标志
    bool is_calculated_ = false;
    // 单方向加速度数据
    data_structure::Acceleration input_acceleration_{};
    // 建筑信息
    std::shared_ptr<data_structure::Building> building_ptr_{};
    // 参数识别结果
    PiResult pi_result_{};
};

} // namespace parameters_identification

#endif // PARAMETERS_IDENTIFICATION_BASIC_PARAMETERS_IDENTIFICATION_H