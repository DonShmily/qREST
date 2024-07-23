/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/numerical_algorithm/vector_calculation.h
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
// Description:
// 数值算法：向量计算，拓展std::vector的操作

#ifndef NUMERICAL_ALGORITHM_VECTOR_CALCULATION_H_
#define NUMERICAL_ALGORITHM_VECTOR_CALCULATION_H_

// stdc++ headers
#include <cstdlib>
#include <stdexcept>
#include <vector>


namespace numerical_algorithm
{

// 寻找向量中的极值点
// @param vec 向量
// @return 极值点的索引
inline std::vector<std::size_t> FindPeaks(const std::vector<double> &vec)
{
    std::vector<std::size_t> peaks;
    for (std::size_t i = 1; i < vec.size() - 1; ++i)
    {
        if (vec[i] > vec[i - 1] && vec[i] > vec[i + 1])
        {
            peaks.push_back(i);
        }
    }
    return peaks;
}

inline std::vector<double> VectorOperation(const std::vector<double> &vector1,
                                           const std::vector<double> &vector2,
                                           const char operation)
{
    auto vec_size = vector1.size();
    std::vector<double> result(vec_size);
    switch (operation)
    {
        case '+':
            for (std::size_t i = 0; i < vec_size; ++i)
            {
                result[i] = vector1[i] + vector2[i];
            }
            break;
        case '-':
            for (std::size_t i = 0; i < vec_size; ++i)
            {
                result[i] = vector1[i] - vector2[i];
            }
            break;
        case '*':
            for (std::size_t i = 0; i < vec_size; ++i)
            {
                result[i] = vector1[i] * vector2[i];
            }
            break;
        case '/':
            for (std::size_t i = 0; i < vec_size; ++i)
            {
                result[i] = vector1[i] / vector2[i];
            }
            break;
        default:
            throw std::invalid_argument("Invalid operation");
            break;
    }
    return result;
}

inline std::vector<double> VectorOperation(const std::vector<double> &vector1,
                                           const double scalar,
                                           const char operation)
{
    auto vec_size = vector1.size();
    std::vector<double> result(vec_size);
    switch (operation)
    {
        case '+':
            for (std::size_t i = 0; i < vec_size; ++i)
            {
                result[i] = vector1[i] + scalar;
            }
            break;
        case '-':
            for (std::size_t i = 0; i < vec_size; ++i)
            {
                result[i] = vector1[i] - scalar;
            }
            break;
        case '*':
            for (std::size_t i = 0; i < vec_size; ++i)
            {
                result[i] = vector1[i] * scalar;
            }
            break;
        case '/':
            for (std::size_t i = 0; i < vec_size; ++i)
            {
                result[i] = vector1[i] / scalar;
            }
            break;
        default:
            throw std::invalid_argument("Invalid operation");
            break;
    }
    return result;
}

// 寻找向量绝对值最大的元素
// @param vec 向量
// @return 绝对值最大的元素
inline double FindMaxAbs(const std::vector<double> &input_vector)
{
    return std::abs(*std::max_element(
        input_vector.begin(), input_vector.end(), [](double a, double b) {
            return std::abs(a) < std::abs(b);
        }));
}
} // namespace numerical_algorithm

#endif // NUMERICAL_ALGORITHM_VECTOR_CALCULATION_H_