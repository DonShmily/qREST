/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/numerical_algorithm/filter.h
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
// Description:
// 数值算法：filter滤波方法类

#ifndef SRC_NUMERICAL_ALGORITHM_FILTER_H_
#define SRC_NUMERICAL_ALGORITHM_FILTER_H_

// stdc++ headers
#include <vector>

// third-party headers

// project headers
#include "basic_filtering.h"
#include "butterworth_filter_design.h"


namespace numerical_algorithm
{

// Filter滤波方法类
class Filter : public BasicFiltering
{
public:
    // 默认构造函数
    Filter() = default;
    // 由两个std::vector参数构造
    // @param coefficients_a 滤波器分母系数
    // @param coefficients_b 滤波器分子系数
    Filter(std::vector<double> coefficients_a,
           std::vector<double> coefficients_b)
        : coefficients_a_(coefficients_a), coefficients_b_(coefficients_b)
    {}
    // 由一个std::vector作为参数b构造，a默认为1
    // @param coefficients_b 滤波器分子系数
    explicit Filter(std::vector<double> coefficients_b)
        : coefficients_a_(1, 1.0), coefficients_b_(coefficients_b)
    {}
    // 由butterworth滤波器设计构造
    // @param filter_design butterworth滤波器设计类
    explicit Filter(const ButterworthFilterDesign &filter_design)
    {
        filter_design.get_filter_coefficients(coefficients_a_, coefficients_b_);
    }

    // 拷贝构造函数
    Filter(const Filter &filter) = default;
    // 移动构造函数
    Filter(Filter &&filter) = default;
    // 析构函数
    ~Filter() = default;

    // 滤波算法的入口
    // @param input_signal 输入信号矩阵
    // @param output_signal 输出信号矩阵
    void Filtering(const Eigen::MatrixXd &input_signal,
                   Eigen::MatrixXd &output_signal) override;

private:
    // filter滤波方法参数
    std::vector<double> coefficients_a_, coefficients_b_;

    // 有限脉冲响应滤波器
    void fir_filter(const Eigen::MatrixXd &input_signal,
                    Eigen::MatrixXd &output_signal);
    // 无限脉冲响应滤波器
    void iir_filter(const Eigen::MatrixXd &input_signal,
                    Eigen::MatrixXd &output_signal);
};

} // namespace numerical_algorithm

#endif // SRC_NUMERICAL_ALGORITHM_FILTER_H_