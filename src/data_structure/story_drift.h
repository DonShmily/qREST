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

// third-party library headers
#include <eigen3/Eigen/Dense>


// project headers


namespace data_structure
{
// 层间位移角信息类
class StoryDrift
{
public:
    // 构造函数
    StoryDrift() = default;
    // 从Eigen::MatrixXd构造
    StoryDrift(const Eigen::MatrixXd &story_drift) : story_drift_(story_drift)
    {}
    // 拷贝构造函数
    StoryDrift(const StoryDrift &story_drift) = default;
    // 移动构造函数
    StoryDrift(StoryDrift &&story_drift) = default;
    // 析构函数
    ~StoryDrift() = default;

    // 获取层间位移角
    const Eigen::MatrixXd &get_story_drift() const { return story_drift_; }

    // 获取层间位移角的引用
    Eigen::MatrixXd &data() { return story_drift_; }

private:
    // 层间位移角
    Eigen::MatrixXd story_drift_{};
};

} // namespace data_structure

#endif // SRC_DATA_STRUCTURE_STORY_DRIFT_H_