/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/numerical_algorithm/basic_filter_design.h
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
// Description:
// 数值算法：滤波器设计基类

// TODO:滤波器设计类如何方便地传递给滤波器？

#ifndef SRC_NUMERICAL_ALGORITHM_BASIC_FILTER_DESIGN_H_
#define SRC_NUMERICAL_ALGORITHM_BASIC_FILTER_DESIGN_H_

// stdc++ headers
#include <vector>

namespace numerical_algorithm
{

// 滤波器类型（幅频特性分类）
enum class FilterType
{
    lowpass,
    highpass,
    bandpass
};

// 滤波器设计方法
enum class FilterGenerator
{
    butter
};

// 滤波器设计基类
class BasicFilterDesign
{
public:
    // 默认构造函数
    BasicFilterDesign() = default;
    // 带通滤波器构造函数
    // @param filter_order 滤波器阶数
    // @param low_frequency 低频截止频率
    // @param high_frequency 高频截止频率
    // @param filter_type 滤波器类型（默认带通）
    BasicFilterDesign(int filter_order,
                      double low_frequency,
                      double high_frequency,
                      FilterType filter_type = FilterType::bandpass);
    // 低通/高通滤波器构造函数（默认低通）
    // @param filter_order 滤波器阶数
    // @param frequency 截止频率
    // @param filter_type 滤波器类型（默认低通）
    BasicFilterDesign(int filter_order,
                      double frequency,
                      FilterType filter_type = FilterType::lowpass);
    // 拷贝构造函数
    BasicFilterDesign(const BasicFilterDesign &filter_design) = default;
    // 移动构造函数
    BasicFilterDesign(BasicFilterDesign &&filter_design) = default;
    // 析构函数
    virtual ~BasicFilterDesign() = default;

    // 设置滤波器阶数
    // @param filter_order 滤波器阶数
    void set_filter_order(int filter_order) { filter_order_ = filter_order; }
    // 设置滤波器截止频率
    // @param low_frequency 低频截止频率
    // @param high_frequency 高频截止频率
    void set_frequency(double low_frequency, double high_frequency)
    {
        low_frequency_ = low_frequency;
        high_frequency_ = high_frequency;
    }
    // 滤波器设计函数
    virtual void DesignFilter() = 0;

protected:
    // 滤波器阶数
    int filter_order_{};
    // 滤波器截止频率
    double low_frequency_{}, high_frequency_{};
    // 滤波器类型
    FilterType filter_type_{};
};
} // namespace numerical_algorithm


#endif