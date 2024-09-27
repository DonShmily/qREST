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
** Last Modified: Saturday, 10th August 2024 22:26:40
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 工程需求参量计算方法：滤波积分插值法

#ifndef EDP_CALCULATION_FILTERING_INTERP_H_
#define EDP_CALCULATION_FILTERING_INTERP_H_

// stdc++ headers
#include <memory>

// third-party library headers

// project-specific headers
#include "basic_edp_calculation.h"

#include "data_structure/acceleration.h"
#include "data_structure/building.h"

#include "numerical_algorithm/basic_filter_design.h"
#include "numerical_algorithm/basic_filtering.h"
#include "numerical_algorithm/interp.h"


namespace edp_calculation
{

// 滤波积分插值法计算工程需求参量的方法参数结构体，默认(*)
struct FilteringIntegralMethod
{
    // 滤波器阶数
    int filter_order_{2};
    // 滤波器截止频率
    double low_frequency_{0.1}, high_frequency_{20};
    // 滤波器类型：
    // lowpass：低通滤波；
    // highpass：高通滤波；
    // *bandpass：带通滤波；
    numerical_algorithm::FilterType filter_type_ =
        numerical_algorithm::FilterType::bandpass;
    // 滤波函数：
    // filter：单向滤波；
    // *filtfilt：零相位双向滤波
    numerical_algorithm::FilterFunction filter_function_ =
        numerical_algorithm::FilterFunction::filtfilt;
    // 滤波器类型：
    // *butter：巴特沃斯滤波器；
    numerical_algorithm::FilterGenerator filter_generator_ =
        numerical_algorithm::FilterGenerator::butter;
    // 插值方法：
    // Linear：线性插值；
    // *CubicSpline：三次样条插值；
    // Akima：Akima插值；
    // Steffen：Steffen插值；
    // Polynomial：多项式插值
    numerical_algorithm::InterpType interp_type_ =
        numerical_algorithm::InterpType::CubicSpline;
}; // struct FilteringIntegralMethod

// 滤波积分插值法计算工程需求参量的类
class FilteringIntegral : public BasicEdpCalculation
{
public:
    // 默认构造函数
    FilteringIntegral() = default;

    // 从配置文件中读取参数构造
    // @param acceleration 加速度数据
    // @param building 建筑结构信息
    FilteringIntegral(const data_structure::Acceleration &acceleration,
                      data_structure::Building &building)
        : BasicEdpCalculation(acceleration, building)
    {
        LoadConfig();
    }

    // 从加速度数据中构造，默认使用巴特沃斯滤波器，零相位双向滤波，带通滤波
    // @param acceleration 加速度数据
    // @param building 建筑结构信息
    // @param filter_order 滤波器阶数
    // @param low_frequency 滤波器低频截止频率
    // @param high_frequency 滤波器高频截止频率
    FilteringIntegral(const data_structure::Acceleration &acceleration,
                      data_structure::Building &building,
                      int filter_order,
                      double low_frequency,
                      double high_frequency)
        : BasicEdpCalculation(acceleration, building)
    {
        method_.filter_order_ = filter_order;
        method_.low_frequency_ = low_frequency;
        method_.high_frequency_ = high_frequency;
    }

    // 从加速度数据指针中构造，默认使用巴特沃斯滤波器，零相位双向滤波，带通滤波
    // @param acceleration_ptr 加速度数据指针
    // @param building_ptr 建筑结构信息指针
    // @param filter_order 滤波器阶数
    // @param low_frequency 滤波器低频截止频率
    // @param high_frequency 滤波器高频截止频率
    FilteringIntegral(
        std::shared_ptr<const data_structure::Acceleration> acceleration_ptr,
        std::shared_ptr<data_structure::Building> building_ptr,
        int filter_order,
        double low_frequency,
        double high_frequency)
        : BasicEdpCalculation(acceleration_ptr, building_ptr)
    {
        method_.filter_order_ = filter_order;
        method_.low_frequency_ = low_frequency;
        method_.high_frequency_ = high_frequency;
    }

    // 析构函数
    ~FilteringIntegral() = default;

    // 设置滤波器阶数
    // @param filter_order 滤波器阶数
    void set_filter_order(int filter_order)
    {
        method_.filter_order_ = filter_order;
    }

    // 设置滤波器截止频率
    // @param low_frequency 滤波器低频截止频率
    // @param high_frequency 滤波器高频截止频率
    void set_filter_frequency(double low_frequency, double high_frequency)
    {
        method_.low_frequency_ = low_frequency;
        method_.high_frequency_ = high_frequency;
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
    EdpResult &get_result() { return *result_; }

    // 获取滤波积分插值法计算结果的指针
    // @return 滤波积分插值法计算结果的指针
    std::shared_ptr<EdpResult> get_result_ptr() { return result_; }

private:
    // 滤波积分插值法计算方法参数
    FilteringIntegralMethod method_{};
    // 计算结果的指针
    std::shared_ptr<EdpResult> result_{};
};
} // namespace edp_calculation


#endif // EDP_CALCULATION_FILTERING_INTERP_H_