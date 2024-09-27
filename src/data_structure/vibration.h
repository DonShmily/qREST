/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\data_structure\vibration.h
** -----
** File Created: Thursday, 19th September 2024 14:11:19
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Thursday, 19th September 2024 14:11:41
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/
// Description:振动时程信息类，以组合的形式同时记录了加速度、速度和位移

#ifndef DATA_STRUCTURE_VIBRATION_H
#define DATA_STRUCTURE_VIBRATION_H

// std headers
#include <memory>

// project headers
#include "basic_data_structure.h"


namespace data_structure
{
// 振动时程信息结构体，一般仅用于储存数据
struct Vibration
{
    // 加速度、速度和位移数据
    BasicData acceleration{}, velocity{}, displacement{};
    // 频率
    double frequency{};
};
} // namespace data_structure

#endif // DATA_STRUCTURE_VIBRATION_H