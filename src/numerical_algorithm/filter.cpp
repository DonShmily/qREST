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
// 数值算法：filter滤波方法类的实现

// associated header
#include "filter.h"

// stdc++ headers

// third-party headers

// project headers

namespace numerical_algorithm
{

void Filter::Filtering(const Eigen::MatrixXd &input_signal,
                       Eigen::MatrixXd &output_signal)
{
    if (coefficients_a_.empty() || coefficients_b_.empty())
    {
        throw std::runtime_error("Filter coefficients are empty.");
    }
    if (input_signal.rows() == 0 || input_signal.cols() == 0)
    {
        throw std::runtime_error("Input signal is empty.");
    }
    if (coefficients_a_.front() == 0)
    {
        throw std::runtime_error("Filter coefficients are invalid.");
    }

    if (coefficients_a_.size() == 1)
    {
        fir_filter(input_signal, output_signal);
    }
    else { iir_filter(input_signal, output_signal); }
}

// FIR滤波算法
void Filter::fir_filter(const Eigen::MatrixXd &input_signal,
                        Eigen::MatrixXd &output_signal)
{
    output_signal =
        Eigen::MatrixXd::Zero(input_signal.rows(), input_signal.cols());
    for (int i = 0; i < input_signal.rows(); ++i)
    {
        for (int j = 0; j < input_signal.cols(); ++j)
        {
            for (int k = 0; k < coefficients_b_.size(); ++k)
            {
                if (i >= k)
                {
                    output_signal(i, j) +=
                        coefficients_b_[k] * input_signal(i - k, j);
                }
            }
        }
    }
}

// IIR滤波算法
void Filter::iir_filter(const Eigen::MatrixXd &input_signal,
                        Eigen::MatrixXd &output_signal)
{
    output_signal =
        Eigen::MatrixXd::Zero(input_signal.rows(), input_signal.cols());
    for (int i = 0; i < input_signal.rows(); ++i)
    {
        for (int j = 0; j < input_signal.cols(); ++j)
        {
            for (int k = 0; k < coefficients_a_.size(); ++k)
            {
                if (i >= k)
                {
                    output_signal(i, j) +=
                        coefficients_a_[k] * input_signal(i - k, j);
                }
            }
            for (int k = 1; k < coefficients_b_.size(); ++k)
            {
                if (i >= k)
                {
                    output_signal(i, j) -=
                        coefficients_b_[k] * output_signal(i - k, j);
                }
            }
        }
    }
}

} // namespace numerical_algorithm