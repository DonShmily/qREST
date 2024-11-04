/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\edp_calculation\modified_filtering_integral.h
** -----
** File Created: Monday, 15th July 2024 14:32:17
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Monday, 4th November 2024 00:35:12
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 工程需求参量计算方法：改进的滤波积分插值法，基于最大功率比的滤波方法
// 算法提供者：2021级硕士陈鹏杰（Chen Pengjie）

#ifndef EDP_CALCULATION_MODIFIED_FILTERING_INTEGRAL_H_
#define EDP_CALCULATION_MODIFIED_FILTERING_INTEGRAL_H_

// stdc++ headers
#include <memory>
#include <vector>

// project headers
#include "data_structure/acceleration.h"
#include "data_structure/building.h"
#include "data_structure/vibration.h"

#include "basic_edp_calculation.h"
#include "filtering_integral.h"


namespace edp_calculation
{

// 滤波积分插值法计算工程需求参量的类
class ModifiedFilteringIntegral : public BasicEdpCalculation
{
public:
    // 默认构造函数
    ModifiedFilteringIntegral() = default;

    // 从加速度数据和建筑信息构造对象
    // @param acceleration 加速度数据
    // @param building_ptr 建筑信息
    // @param config_ptr 配置信息
    ModifiedFilteringIntegral(
        const data_structure::Acceleration &acceleration,
        const std::shared_ptr<data_structure::Building> &building_ptr,
        const std::shared_ptr<settings::Config> &config_ptr)
        : BasicEdpCalculation(acceleration, building_ptr, config_ptr)
    {}

    // 从加速度数据指针和建筑信息构造对象
    // @param acceleration_ptr 加速度数据指针
    // @param building_ptr 建筑信息
    // @param config_ptr 配置信息
    ModifiedFilteringIntegral(
        const std::shared_ptr<data_structure::Acceleration> &acceleration_ptr,
        const std::shared_ptr<data_structure::Building> &building_ptr,
        const std::shared_ptr<settings::Config> &config_ptr)
        : BasicEdpCalculation(acceleration_ptr, building_ptr, config_ptr)
    {}

    // 析构函数
    ~ModifiedFilteringIntegral() = default;

    // 设置滤波器阶数
    // @param filter_order 滤波器阶数
    void set_filter_order(int filter_order)
    {
        parameter_.filter_order_ = filter_order;
    }

    // 滤波积分插值法计算的入口
    void CalculateEdp() override;

private:
    // 滤波积分插值法计算单列加速度
    void CalculateSingle(const std::size_t &col);
};
} // namespace edp_calculation

#endif // EDP_CALCULATION_MODIFIED_FILTERING_INTEGRAL_H_