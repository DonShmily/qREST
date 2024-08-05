/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/data_structure/inter_story_drift.h
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
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

    // 拷贝构造函数
    InterStoryDrift(const InterStoryDrift &inter_story_drift) = default;

    // 移动构造函数
    InterStoryDrift(InterStoryDrift &&inter_story_drift) = default;

    // 拷贝赋值函数
    InterStoryDrift &
    operator=(const InterStoryDrift &inter_story_drift) = default;

    // 析构函数
    ~InterStoryDrift() = default;

private:
};

} // namespace data_structure

#endif // DATA_STRUCTURE_INTER_STORY_DRIFT_H_