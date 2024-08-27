/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\data_structure\displacement.cpp
** -----
** File Created: Tuesday, 9th July 2024 14:14:32
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Saturday, 10th August 2024 22:25:14
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 位移信息类的实现。

// associated header
#include "displacement.h"

namespace data_structure
{

// 求解楼层相对底层的位移
Displacement Displacement::relative_displacement() const
{
    Displacement result(data_->front().size(), data_->size() - 1, frequency_);
    for (std::size_t i = 0; i < result.data_->size(); ++i)
    {
        for (std::size_t j = 0; j < result.data_->at(i).size(); ++j)
        {
            result.data_->at(i)[j] = data_->at(i + 1)[j] - data_->at(0)[j];
        }
    }
    return result;
}

// 求解层间相对位移
Displacement Displacement::interstory_displacement() const
{
    Displacement result(data_->front().size(), data_->size() - 1, frequency_);
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