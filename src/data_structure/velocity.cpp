/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/data_structure/displacement.cpp
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
// Description:
// 位移信息类的实现。

// associated header
#include "velocity.h"

namespace data_structure
{

// 求解楼层相对底层的速度
Velocity Velocity::relative_velocity() const
{
    Velocity result(data_.front().size(), data_.size() - 1, frequency_);
    for (std::size_t i = 0; i < result.data_.size(); ++i)
    {
        for (std::size_t j = 0; j < result.data_[i].size(); ++j)
        {
            result.data_[i][j] = data_[i + 1][j] - data_[0][j];
        }
    }
    return result;
}

// 求解层间相对速度
Velocity Velocity::interstory_velocity() const
{
    Velocity result(data_.front().size(), data_.size() - 1, frequency_);
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