/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/gmp_calculation/gmp_caculation.cpp
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
// Description:
// 计算地震参数类的实现。

#include "gmp_caculation.h"

namespace gmp_calculation
{

// NewmakeBeta方法计算响应
ResponseSpectrumTi GmpCalculation::NewmarkBeta(const double &Ti)
{
    double dt = 1.0 / frequency_;
    double beta = 0.25, gamma = 0.5;
    double omega = 2 * M_PI / Ti;
    double k = omega * omega, c = 2 * damping_ratio_ * omega;
    double p1 = 1 / (beta * dt * dt);
    double p2 = gamma / (beta * dt);
    double p3 = 1 / (beta * dt);
    double p4 = gamma / beta - 1;
    double p5 = 1 / (2 * beta) - 1;
    double p6 = dt / 2 * (gamma / beta - 2);
    double a1 = p1 + p2 * c, a2 = p3 + p4 * c, a3 = p5 + p6 * c;
    double k_b = k + a1;
    // 初始化
    std::vector<double> u(acceleration_ptr_->size()),
        v(acceleration_ptr_->size()), a(acceleration_ptr_->size());
    double p = -acceleration_ptr_->at(0);
    u[0] = p / k_b;
    v[0] = gamma * p1 * u[0];
    a[0] = p1 * u[0];
    // 迭代
    for (size_t i = 1; i != acceleration_ptr_->size(); i++)
    {
        p = -acceleration_ptr_->at(i) + a1 * u[i - 1] + a2 * v[i - 1]
            + a3 * a[i - 1];
        u[i] = p / k_b;
        v[i] = gamma * p1 * (u[i] - u[i - 1]) - p4 * v[i - 1] - p6 * a[i - 1];
        a[i] = p1 * (u[i] - u[i - 1]) - p3 * v[i - 1] - p6 * a[i - 1] / dt;
    }
    // 计算结果
    response_spectrum_ti_.SaTi = numerical_algorithm::FindMaxAbs(a);
    response_spectrum_ti_.SvTi = numerical_algorithm::FindMaxAbs(v);
    response_spectrum_ti_.SdTi = numerical_algorithm::FindMaxAbs(u);
    return response_spectrum_ti_;
}

} // namespace gmp_calculation