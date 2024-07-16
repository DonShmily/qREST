/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/data_structure/acceleration.cpp
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
// Description:
// 加速度信息类的实现。

// associated header
#include <algorithm>
#include <cstddef>
#include <numeric>

#include "acceleration.h"


namespace data_structure
{

// 调整输入加速度使其各列均值为0
void Acceleration::adjust_mean()
{
    for (std::size_t i = 0; i < data_.size(); ++i)
    {
        const double col_mean =
            std::accumulate(data_[i].begin(), data_[i].end(), 0.0)
            / data_[i].size();
        std::transform(
            data_[i].begin(),
            data_[i].end(),
            data_[i].begin(),
            [col_mean](const double &val) { return val - col_mean; });
    }
}

// 求解楼层相对底层的加速度
Acceleration Acceleration::relative_acceleration() const
{
    Acceleration result(data_.front().size(), data_.size() - 1, frequency_);
    for (std::size_t i = 0; i < result.data_.size(); ++i)
    {
        for (std::size_t j = 0; j < result.data_[i].size(); ++j)
        {
            result.data_[i][j] = data_[i + 1][j] - data_[0][j];
        }
    }
    return result;
}

// 求解层间相对加速度
Acceleration Acceleration::interstory_acceleration() const
{
    Acceleration result(data_.front().size(), data_.size() - 1, frequency_);
    for (std::size_t i = 0; i < result.data_.size(); ++i)
    {
        for (std::size_t j = 0; j < result.data_[i].size(); ++j)
        {
            result.data_[i][j] = data_[i + 1][j] - data_[i][j];
        }
    }
    return result;
}

} // namespace data_structure