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
#include "acceleration.h"


namespace data_structure
{

// 调整输入加速度使其各列均值为0
void Acceleration::adjust_mean()
{
    for (std::size_t i = 0; i < data_.cols(); ++i)
    {
        const double col_mean = data_.col(i).mean();
        data_.col(i).array() -= col_mean;
    }
}

// 求解楼层相对底层的加速度
Acceleration Acceleration::relative_acceleration() const
{
    Acceleration result(data_.cols(), data_.rows() - 1, frequency_);
    for (std::size_t i = 0; i < result.data_.cols(); ++i)
    {
        result.data_.col(i) = data_.col(i + 1) - data_.col(0);
    }
    return result;
}

// 求解层间相对加速度
Acceleration Acceleration::interstory_acceleration() const
{
    Acceleration result(data_.cols(), data_.rows() - 1, frequency_);
    for (std::size_t i = 0; i < result.data_.cols(); ++i)
    {
        result.data_.col(i) = data_.col(i + 1) - data_.col(i);
    }
    return result;
}

} // namespace data_structure