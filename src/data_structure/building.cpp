/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\data_structure\building.cpp
** -----
** File Created: Monday, 15th July 2024 00:51:18
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Saturday, 10th August 2024 22:25:04
** Modified By: Dong Feiyue (donfeiyue@outlook.com>)
*/

// Description:
// 建筑信息类的实现

// associated header
#include "building.h"

// stdc++ headers
#include <vector>


namespace data_structure
{

// 计算建筑层间高度
void Building::calculate_inter_height()
{
    inter_height_.resize(floor_height_.size() - 1);
    for (size_t i = 0; i < inter_height_.size(); ++i)
    {
        inter_height_[i] = floor_height_[i + 1] - floor_height_[i];
    }
}

} // namespace data_structure