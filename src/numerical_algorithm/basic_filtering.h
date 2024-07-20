/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/numerical_algorithm/basic_filtering.h
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
// Description:
// 数值算法：滤波算法基类

#ifndef NUMERICAL_ALGORITHM_BASIC_FILTERING_H_
#define NUMERICAL_ALGORITHM_BASIC_FILTERING_H_

// stdc++ headers
#include <memory>
#include <vector>

// project headers
#include "basic_filter_design.h"


namespace numerical_algorithm
{

// 滤波函数类型
enum class FilterFunction
{
    filter,
    filtfilt
};

// 滤波算法基类
class BasicFiltering
{
public:
    // 默认构造函数
    BasicFiltering() = default;
    // 拷贝构造函数
    BasicFiltering(const BasicFiltering &filtering) = default;
    // 移动构造函数
    BasicFiltering(BasicFiltering &&filtering) = default;
    // 析构函数
    virtual ~BasicFiltering() = default;

    // 矩阵滤波算法入口
    // @param input_signal 输入信号矩阵
    // @return 滤波后的信号矩阵
    virtual std::vector<std::vector<double>>
    Filtering(const std::vector<std::vector<double>> &input_signa) = 0;

    // 单列数据滤波算法入口
    // @param input_signal 输入信号
    // @return 滤波后的信号
    virtual std::vector<double>
    Filtering(const std::vector<double> &input_signal) = 0;

protected:
};

} // namespace numerical_algorithm

#endif // NUMERICAL_ALGORITHM_BASIC_FILTERING_H_
