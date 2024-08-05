/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/data_structure/building.cpp
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
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