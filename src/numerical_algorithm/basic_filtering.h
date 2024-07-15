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

#ifndef SRC_NUMERICAL_ALGORITHM_BASIC_FILTERING_H_
#define SRC_NUMERICAL_ALGORITHM_BASIC_FILTERING_H_

// stdc++ headers
#include <memory>
#include <vector>

// third-party headers
#include "eigen3/Eigen/Core"

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

    // 滤波算法入口
    // @param input_signal 输入信号矩阵
    // @param output_signal 输出信号矩阵
    virtual void Filtering(const Eigen::MatrixXd &input_signal,
                           Eigen::MatrixXd &output_signal) = 0;

protected:
};

} // namespace numerical_algorithm

#endif // SRC_NUMERICAL_ALGORITHM_BASIC_FILTERING_H_
