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
#include "displacement.h"

namespace data_structure
{

// 求解楼层相对底层的位移
Displacement Displacement::relative_displacement() const
{
    Displacement result(data_.cols(), data_.rows() - 1, frequency_);
    for (std::size_t i = 0; i < result.data_.cols(); ++i)
    {
        result.data_.col(i) = data_.col(i + 1) - data_.col(0);
    }
    return result;
}

// 求解层间相对位移
Displacement Displacement::interstory_displacement() const
{
    Displacement result(data_.cols(), data_.rows() - 1, frequency_);
    for (std::size_t i = 0; i < result.data_.cols(); ++i)
    {
        result.data_.col(i) = data_.col(i + 1) - data_.col(i);
    }
    return result;
}
} // namespace data_structure