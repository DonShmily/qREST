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
** Last Modified: Saturday, 10th August 2024 22:27:00
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

    // 从配置文件中读取参数构造
    // @param acceleration 加速度数据
    // @param building 建筑结构信息
    ModifiedFilteringIntegral(const data_structure::Acceleration &acceleration,
                              data_structure::Building &building)
        : BasicEdpCalculation(acceleration, building)
    {
        LoadConfig();
    }

    // 从加速度数据中构造，默认使用巴特沃斯滤波器，零相位双向滤波，带通滤波
    // @param acceleration 加速度数据
    // @param building 建筑结构信息
    // @param filter_order 滤波器阶数
    ModifiedFilteringIntegral(const data_structure::Acceleration &acceleration,
                              data_structure::Building &building,
                              int filter_order)
        : BasicEdpCalculation(acceleration, building)
    {
        method_.filter_order_ = filter_order;
    }

    // 从加速度数据指针中构造，默认使用巴特沃斯滤波器，零相位双向滤波，带通滤波
    // @param acceleration_ptr 加速度数据指针
    // @param building_ptr 建筑结构信息指针
    // @param filter_order 滤波器阶数
    ModifiedFilteringIntegral(
        std::shared_ptr<const data_structure::Acceleration> acceleration_ptr,
        std::shared_ptr<data_structure::Building> building_ptr,
        int filter_order)
        : BasicEdpCalculation(acceleration_ptr, building_ptr)
    {
        method_.filter_order_ = filter_order;
    }

    // 析构函数
    ~ModifiedFilteringIntegral() = default;

    // 设置滤波器阶数
    // @param filter_order 滤波器阶数
    void set_filter_order(int filter_order)
    {
        method_.filter_order_ = filter_order;
    }

    // 从配置文件中读取参数
    void LoadConfig(
        const std::string &config_file = "config/EDP_Config.json") override;

    // 获取滤波积分插值法计算方法参数
    // @return 滤波积分插值法计算方法参数的引用
    FilteringIntegralMethod &get_filtering_interp_method() { return method_; }

    // 滤波积分插值法计算的入口
    void CalculateEdp() override;

    // 获取滤波积分插值法计算结果
    // @return 滤波积分插值法计算结果的引用
    InterStoryDriftResult &get_filtering_interp_result() { return result_; }

private:
    // 滤波积分插值法计算方法参数
    FilteringIntegralMethod method_{};
    // 计算结果
    InterStoryDriftResult result_{};

    // 滤波积分插值法计算单列加速度
    std::vector<double> CalculateSingle(const std::size_t &col);
};
} // namespace edp_calculation

#endif // EDP_CALCULATION_MODIFIED_FILTERING_INTEGRAL_H_