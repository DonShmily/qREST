/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\parameters_identification\simple_parameters_identification.cpp
** -----
** File Created: Monday, 30th September 2024 16:45:33
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Monday, 30th September 2024 22:04:50
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:简单参数识别类的实现。

// associated header
#include "simple_parameters_identification.h"

// stdc++ headers
#include <algorithm>
#include <cstddef>
#include <vector>

// project headers
#include "numerical_algorithm/vector_calculation.h"

namespace parameters_identification
{
void SimpleParametersIdentification::Identify()
{
    int lenth = input_acceleration_.get_row_number();
    // 取读取的加速度最高层的加速度时程
    std::vector<double> top_acc =
        input_acceleration_.get_col(input_acceleration_.get_col_number() - 1);
    double freq = input_acceleration_.get_frequency(), dt = 1.0 / freq;
    // 计算加速度时程的频谱
    auto fft_result = numerical_algorithm::FourierTransform(top_acc);
    std::vector<double> fft_result_abs(top_acc.size(), 0);
    for (std::size_t i = 0; i < fft_result.size(); ++i)
    {
        fft_result_abs[i] = std::abs(fft_result[i]);
    }

    // 计算前两阶频率
    std::size_t peak_index;
    auto beg_iter = fft_result_abs.begin();
    // 寻找两个个峰值，具体做法为找最大、去除临近后再查找最大
    for (int i = 0; i < 2; ++i)
    {
        auto max_iter = std::max_element(beg_iter, fft_result_abs.end());
        peak_index = std::distance(fft_result_abs.begin(), max_iter);
        // 去除最大值附近1/100长度的区域
        beg_iter = max_iter + static_cast<int>(lenth / 100);
        // 计算索引相应的频率值
        pi_result_.frequency.push_back(peak_index * freq / lenth);

        // 计算阻尼比
        // 计算半功率带宽
        double half_power = fft_result_abs[peak_index] / std::sqrt(2);
        // 寻找左右两个半功率带宽的边界
        std::size_t left = peak_index, right = peak_index;
        while (fft_result_abs[left] > half_power)
        {
            --left;
        }
        while (fft_result_abs[right] > half_power)
        {
            ++right;
        }
        // 计算半功率带宽
        double half_power_bandwidth = (right - left) * freq / lenth;
        // 计算阻尼比
        pi_result_.damping_ratio.push_back(half_power_bandwidth
                                           / pi_result_.frequency[i]);
    }

    // 已完成参数识别
    is_calculated_ = true;
}
} // namespace parameters_identification