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

#ifndef SRC_NUMERICAL_ALGORITHM_VECTOR_CALCULATION_H_
#define SRC_NUMERICAL_ALGORITHM_VECTOR_CALCULATION_H_

// stdc++ headers
#include <cstddef>
#include <iterator>
#include <vector>


namespace numerical_algorithm
{
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
} // namespace numerical_algorithm

#endif // SRC_NUMERICAL_ALGORITHM_VECTOR_CALCULATION_H_