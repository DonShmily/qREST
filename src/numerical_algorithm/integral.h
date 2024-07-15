/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/numerical_algorithm/integral.h
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
// Description:
// 数值算法：积分算法

#ifndef NUMERICAL_ALGORITHM_INTEGRAL_H_
#define NUMERICAL_ALGORITHM_INTEGRAL_H_

// stdc++ headers
#include <vector>

// third-party library headers
#include "eigen3/Eigen/Dense"

namespace numerical_algorithm
{
// std::vector<double>积分算法
// @param input 输入数据
// @param output 积分结果
// @param dx 积分步长
inline void Cumtrapz(const std::vector<double> &input,
                     std::vector<double> &output,
                     double dx)
{
    output.resize(input.size());
    output[0] = 0.0;
    for (size_t i = 1; i < input.size(); ++i)
    {
        output[i] = output[i - 1] + 0.5 * (input[i] + input[i - 1]) * dx;
    }
}
// Eigen::MaxtrixXd积分算法（按列积分）
// @param input 输入数据矩阵
// @param output 积分结果矩阵
// @param dx 积分步长
inline void
Cumtrapz(const Eigen::MatrixXd &input, Eigen::MatrixXd &output, double dx)
{
    output.resize(input.rows(), input.cols());
    output.row(0).setZero();
    for (int i = 1; i < input.rows(); ++i)
    {
        output.row(i) =
            output.row(i - 1) + 0.5 * (input.row(i) + input.row(i - 1)) * dx;
    }
}
} // namespace numerical_algorithm

#endif // NUMERERICAL_ALGORITHM_INTEGRAL_H_