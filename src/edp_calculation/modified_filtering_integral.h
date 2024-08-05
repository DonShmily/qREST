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
// 算法提供者：2021级硕士陈鹏杰（Chen Pengjie）

#ifndef EDP_CALCULATION_MODIFIED_FILTERING_INTEGRAL_H_
#define EDP_CALCULATION_MODIFIED_FILTERING_INTEGRAL_H_

// stdc++ headers
#include <memory>
#include <vector>

// third-party library headers

// project headers
#include "data_structure/acceleration.h"
#include "data_structure/building.h"

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

    // 从加速度数据中构造，默认使用巴特沃斯滤波器，零相位双向滤波，带通滤波
    // @param acceleration 加速度数据
    // @param filter_order 滤波器阶数
    // @param low_frequency 滤波器低频截止频率
    // @param high_frequency 滤波器高频截止频率
    ModifiedFilteringIntegral(const data_structure::Acceleration &acceleration,
                              data_structure::Building &building,
                              int filter_order)
        : BasicEdpCalculation(acceleration, building),
          filter_order_(filter_order)
    {}

    // 从加速度数据指针中构造，默认使用巴特沃斯滤波器，零相位双向滤波，带通滤波
    // @param acceleration_ptr 加速度数据指针
    // @param filter_order 滤波器阶数
    // @param low_frequency 滤波器低频截止频率
    // @param high_frequency 滤波器高频截止频率
    ModifiedFilteringIntegral(
        std::shared_ptr<const data_structure::Acceleration> acceleration_ptr,
        std::shared_ptr<data_structure::Building> building_ptr,
        int filter_order)
        : BasicEdpCalculation(acceleration_ptr, building_ptr),
          filter_order_(filter_order)
    {}

    // 拷贝构造函数
    ModifiedFilteringIntegral(
        const ModifiedFilteringIntegral &filtering_interp) = default;

    // 移动构造函数
    ModifiedFilteringIntegral(ModifiedFilteringIntegral &&filtering_interp) =
        default;

    // 析构函数
    ~ModifiedFilteringIntegral() = default;

    // 设置滤波器阶数
    // @param filter_order 滤波器阶数
    void set_filter_order(int filter_order) { filter_order_ = filter_order; }

    // 获取滤波积分插值法计算方法参数
    // @return 滤波积分插值法计算方法参数的引用
    FilteringIntegralMethod &get_filtering_interp_method() { return method_; }

    // 滤波积分插值法计算的入口
    void CalculateEdp() override;

    // 获取滤波积分插值法计算结果
    // @return 滤波积分插值法计算结果的引用
    const InterStoryDriftResult &get_filtering_interp_result() const
    {
        return result_;
    }

private:
    // 滤波器阶数
    int filter_order_{};
    // 滤波积分插值法计算方法参数
    FilteringIntegralMethod method_{};
    // 计算结果的指针
    InterStoryDriftResult result_{};

    // 滤波积分插值法计算单列加速度
    std::vector<double> CalculateSingle(const std::size_t &col);
};
} // namespace edp_calculation

#endif // EDP_CALCULATION_MODIFIED_FILTERING_INTEGRAL_H_