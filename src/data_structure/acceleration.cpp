/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\data_structure\acceleration.cpp
** -----
** File Created: Saturday, 6th July 2024 15:15:17
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Saturday, 10th August 2024 22:24:45
** Modified By: Dong Feiyue (donfeiyue@outlook.com>)
*/

// Description:
// 加速度信息类的实现。

// associated header
#include "acceleration.h"

// stdc++ headers
#include <initializer_list>


namespace data_structure
{

// 从指定列构造，从整体的加速度类型变量的部分列构造，col_index为指定列的索引
Acceleration::Acceleration(const Acceleration &acceleration,
                           const std::initializer_list<std::size_t> &col_index,
                           const double &frequency,
                           const double &scale)
    : frequency_(frequency)
{
    resize(acceleration.get_row_number(), col_index.size());
    std::size_t i = 0;
    for (const auto &index : col_index)
    {
        data_->at(i) = acceleration.data_->at(index);
        ++i;
    }
    adjust_acceleration(scale);
}

// 求解楼层相对底层的加速度
Acceleration Acceleration::relative_acceleration() const
{
    Acceleration result(data_->front().size(), data_->size() - 1, frequency_);
    for (std::size_t i = 0; i < result.data_->size(); ++i)
    {
        for (std::size_t j = 0; j < result.data_->at(i).size(); ++j)
        {
            result.data_->at(i)[j] = data_->at(i + 1)[j] - data_->at(0)[j];
        }
    }
    return result;
}

// 求解层间相对加速度
Acceleration Acceleration::interstory_acceleration() const
{
    Acceleration result(data_->front().size(), data_->size() - 1, frequency_);
    for (std::size_t i = 0; i < result.data_->size(); ++i)
    {
        for (std::size_t j = 0; j < result.data_->at(i).size(); ++j)
        {
            result.data_->at(i)[j] = data_->at(i + 1)[j] - data_->at(i)[j];
        }
    }
    return result;
}

} // namespace data_structure