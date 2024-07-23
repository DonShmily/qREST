﻿/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/numerical_algorithm/butterworth_filter_design.h
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
// Description:
// 数值算法：Butterworth滤波器设计类

#ifndef NUMERICAL_ALGORITHM_BUTTERWORTH_FILTER_DESIGN_H_
#define NUMERICAL_ALGORITHM_BUTTERWORTH_FILTER_DESIGN_H_

#define _USE_MATH_DEFINES

// stdc++ headers
#include <cmath>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// project headers
#include "basic_filter_design.h"

namespace numerical_algorithm
{
// Butterworth滤波器设计类
class ButterworthFilterDesign : public BasicFilterDesign
{
public:
    // 默认构造函数
    ButterworthFilterDesign() = default;
    // 通过滤波器阶数和滤波器类型构造
    ButterworthFilterDesign(int filter_order,
                            FilterType filter_type = FilterType::bandpass);
    // 带通滤波器构造函数
    // @param filter_order 滤波器阶数
    // @param low_frequency 低频截止频率
    // @param high_frequency 高频截止频率
    // @param filter_type 滤波器类型（默认带通）
    ButterworthFilterDesign(int filter_order,
                            double low_frequency,
                            double high_frequency,
                            FilterType filter_type = FilterType::bandpass);
    // 低通/高通滤波器构造函数（默认低通）
    // @param filter_order 滤波器阶数
    // @param frequency 截止频率
    // @param filter_type 滤波器类型（默认低通）
    ButterworthFilterDesign(int filter_order,
                            double frequency,
                            FilterType filter_type = FilterType::lowpass);
    // 拷贝构造函数
    ButterworthFilterDesign(const ButterworthFilterDesign &filter_design) =
        default;
    // 移动构造函数
    ButterworthFilterDesign(ButterworthFilterDesign &&filter_design) = default;
    // 析构函数
    ~ButterworthFilterDesign() = default;

    // 获取滤波器参数
    // @param coefficients_a 滤波器分母系数
    // @param coefficients_b 滤波器分子系数
    void get_filter_coefficients(std::vector<double> &coefficients_a,
                                 std::vector<double> &coefficients_b) const
    {
        coefficients_a = coefficients_a_;
        coefficients_b = coefficients_b_;
    }

    // 滤波器设计
    inline void DesignFilter() override
    {
        ComputeDenCoeffs();
        ComputeNumCoeffs();
    }

private:
    // 滤波器参数
    std::vector<double> coefficients_a_{}, coefficients_b_{};

    // 计算滤波器参数的函数
    void ComputeDenCoeffs();
    void ComputeNumCoeffs();
    std::vector<double> TrinomialMultiply(const std::vector<double> &b,
                                          const std::vector<double> &c) const;
    std::vector<double> ComputeLP() const;
    std::vector<double> ComputeHP() const;
};
} // namespace numerical_algorithm

#endif // NUMERICAL_ALGORITHM_BUTTERWORTH_FILTER_DESIGN_H_