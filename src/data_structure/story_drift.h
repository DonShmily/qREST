/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/data_structure/story_drift.h
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
// Description:
// 层间位移角信息类，数据基类的派生，存储层间位移角数据的基本信息。


#ifndef SRC_DATA_STRUCTURE_STORY_DRIFT_H_
#define SRC_DATA_STRUCTURE_STORY_DRIFT_H_

// stdc++ headers

// project headers
#include "basic_data_structure.h"


namespace data_structure
{
// 层间位移角信息类
class StoryDrift : public BasicData
{
public:
    // 构造函数
    StoryDrift() = default;
    // 拷贝构造函数
    StoryDrift(const StoryDrift &story_drift) = default;
    // 移动构造函数
    StoryDrift(StoryDrift &&story_drift) = default;
    // 析构函数
    ~StoryDrift() = default;

private:
};

} // namespace data_structure

#endif // SRC_DATA_STRUCTURE_STORY_DRIFT_H_