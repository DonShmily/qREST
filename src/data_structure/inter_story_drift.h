/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\data_structure\inter_story_drift.h
** -----
** File Created: Monday, 5th August 2024 17:40:31
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Saturday, 10th August 2024 22:25:47
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 层间位移角信息类，数据基类的派生，存储层间位移角数据的基本信息。


#ifndef DATA_STRUCTURE_INTER_STORY_DRIFT_H_
#define DATA_STRUCTURE_INTER_STORY_DRIFT_H_

// stdc++ headers

// project headers
#include "basic_data_structure.h"


namespace data_structure
{
// 层间位移角信息类
class InterStoryDrift : public BasicData
{
public:
    // 构造函数
    InterStoryDrift() = default;

    // 析构函数
    ~InterStoryDrift() = default;

private:
};

} // namespace data_structure

#endif // DATA_STRUCTURE_INTER_STORY_DRIFT_H_