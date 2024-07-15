/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/edp_calculation/modified_filtering_integral.h
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
// Description:
// 工程需求参量计算方法：改进的滤波积分插值法，基于最大功率比的滤波方法
// 仅计算步骤和简单滤波积分方法有差别，成员变量和基本方法相同
// 算法提供者：2021级硕士陈鹏杰（Chen Pengjie）

#ifndef SRC_EDP_CALCULATION_MODIFIED_FILTERING_INTEGRAL_H_
#define SRC_EDP_CALCULATION_MODIFIED_FILTERING_INTEGRAL_H_

// stdc++ headers
#include <cstddef>
#include <memory>

// third-party library headers

// project-specific headers
#include "basic_edp_calculation.h"
#include "filtering_integral.h"

#include "data_structure/acceleration.h"
#include "data_structure/building.h"
#include "data_structure/displacement.h"
#include "data_structure/story_drift.h"
#include "data_structure/velocity.h"

#include "numerical_algorithm/basic_filter_design.h"
#include "numerical_algorithm/basic_filtering.h"
#include "numerical_algorithm/interp.h"


namespace edp_calculation
{

// 滤波积分插值法计算工程需求参量的类
class ModifiedFilteringIntegral : public FilteringIntegral
{
public:
    // 默认构造函数
    ModifiedFilteringIntegral() = default;
    // 从加速度数据中构造，默认使用巴特沃斯滤波器，零相位双向滤波，带通滤波
    // @param acceleration 加速度数据
    // @param filter_order 滤波器阶数
    // @param low_frequency 滤波器低频截止频率
    // @param high_frequency 滤波器高频截止频率
    ModifiedFilteringIntegral(const data_structure::Acceleration &acceleration,
                              data_structure::Building &building,
                              int filter_order,
                              double low_frequency,
                              double high_frequency)
        : FilteringIntegral(acceleration,
                            building,
                            filter_order,
                            low_frequency,
                            high_frequency)

    {}
    // 从加速度数据指针中构造，默认使用巴特沃斯滤波器，零相位双向滤波，带通滤波
    // @param acceleration_ptr 加速度数据指针
    // @param filter_order 滤波器阶数
    // @param low_frequency 滤波器低频截止频率
    // @param high_frequency 滤波器高频截止频率
    ModifiedFilteringIntegral(
        std::shared_ptr<const data_structure::Acceleration> acceleration_ptr,
        std::shared_ptr<data_structure::Building> building_ptr,
        int filter_order,
        double low_frequency,
        double high_frequency)
        : FilteringIntegral(acceleration_ptr,
                            building_ptr,
                            filter_order,
                            low_frequency,
                            high_frequency)
    {}
    // 拷贝构造函数
    ModifiedFilteringIntegral(
        const ModifiedFilteringIntegral &filtering_interp) = default;
    // 移动构造函数
    ModifiedFilteringIntegral(ModifiedFilteringIntegral &&filtering_interp) =
        default;
    // 析构函数
    ~ModifiedFilteringIntegral() = default;

    // 滤波积分插值法计算的入口
    void CalculateEdp();

private:
    // 滤波积分插值法计算单列加速度
    Eigen::MatrixXd CalculateSingle(const std::size_t &col);
};
} // namespace edp_calculation

#endif // SRC_EDP_CALCULATION_MODIFIED_FILTERING_INTEGRAL_H_