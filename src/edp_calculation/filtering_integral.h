/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\edp_calculation\filtering_integral.h
** -----
** File Created: Friday, 12th July 2024 00:13:26
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Monday, 4th November 2024 00:34:57
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 工程需求参量计算方法：滤波积分插值法

#ifndef EDP_CALCULATION_FILTERING_INTERP_H_
#define EDP_CALCULATION_FILTERING_INTERP_H_

// stdc++ headers
#include <memory>

// project-specific headers
#include "basic_edp_calculation.h"
#include "data_structure/acceleration.h"
#include "data_structure/building.h"
#include "numerical_algorithm/basic_filter_design.h"
#include "numerical_algorithm/basic_filtering.h"
#include "numerical_algorithm/interp.h"


namespace edp_calculation
{
// 滤波积分插值法计算工程需求参量的类
class FilteringIntegral : public BasicEdpCalculation
{
public:
    // 默认构造函数
    FilteringIntegral() = default;

    // 从加速度数据和建筑信息构造
    // @param acceleration 加速度数据
    // @param building_ptr 建筑信息指针
    // @param config 配置信息
    FilteringIntegral(
        const data_structure::Acceleration &acceleration,
        const std::shared_ptr<data_structure::Building> &building_ptr,
        std::shared_ptr<settings::Config> config)
        : BasicEdpCalculation(acceleration, building_ptr, config)
    {}

    // 从加速度数据指针和建筑信息构造
    // @param acceleration_ptr 加速度数据指针
    // @param building_ptr 建筑信息指针
    // @param config 配置信息
    FilteringIntegral(
        const std::shared_ptr<data_structure::Acceleration> &acceleration_ptr,
        const std::shared_ptr<data_structure::Building> &building_ptr,
        std::shared_ptr<settings::Config> config)
        : BasicEdpCalculation(acceleration_ptr, building_ptr, config)
    {}

    // 析构函数
    ~FilteringIntegral() = default;

    // 设置滤波器阶数
    // @param filter_order 滤波器阶数
    void set_filter_order(int filter_order)
    {
        parameter_.filter_order_ = filter_order;
    }

    // 设置滤波器截止频率
    // @param low_frequency 滤波器低频截止频率
    // @param high_frequency 滤波器高频截止频率
    void set_filter_frequency(double low_frequency, double high_frequency)
    {
        parameter_.low_frequency_ = low_frequency;
        parameter_.high_frequency_ = high_frequency;
    }

    // 滤波积分插值法计算的入口
    void CalculateEdp() override;

private:
};
} // namespace edp_calculation


#endif // EDP_CALCULATION_FILTERING_INTERP_H_