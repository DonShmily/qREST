/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\safty_tagging\based_on_inter_story_drift.h
** -----
** File Created: Monday, 5th August 2024 17:40:31
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Saturday, 10th August 2024 22:29:26
** Modified By: Dong Feiyue (donfeiyue@outlook.com>)
*/

// Description:
// 基于层间位移角的安全评价方法。

#ifndef SAFTY_TAGGING_BASED_ON_INTER_STORY_DRIFT_H
#define SAFTY_TAGGING_BASED_ON_INTER_STORY_DRIFT_H

// stdc++ headers
#include <utility>
#include <vector>

// project headers

#include "basic_safty_tagging.h"
#include "edp_calculation/basic_edp_calculation.h"


namespace safty_tagging
{

// 最大层间位移角结果，储存正负向最大层间位移角及对应时间，最大位移及对应时间
struct MaxInterStoryDrift
{
    // 各层最大层间位移角结果及对应时间
    std::vector<std::pair<double, double>> pos_max_inter_story_drift_{},
        neg_max_inter_story_drift_{}, abs_max_inter_story_drift_{};
};

// 最大层间位移角信息，储存最大层间位移角及对应时间、楼层
struct MaxInterStoryDriftMsg
{
    double time;
    size_t floor;
    double inter_story_drift;
};

// 基于层间位移角的安全评价方法
class BasedOnInterStoryDrift : public BasicSaftyTagging
{
public:
    // 默认构造函数
    BasedOnInterStoryDrift() = default;

    // 拷贝构造函数
    BasedOnInterStoryDrift(
        const BasedOnInterStoryDrift &based_on_inter_story_drift) = default;

    // 移动构造函数
    BasedOnInterStoryDrift(
        BasedOnInterStoryDrift &&based_on_inter_story_drift) = default;

    // 从层间位移角计算结果构造
    // @param inter_story_drift 层间位移角计算结果
    explicit BasedOnInterStoryDrift(
        const edp_calculation::InterStoryDriftResult &inter_story_drift)
        : inter_story_drift_(inter_story_drift)
    {}

    // 析构函数
    ~BasedOnInterStoryDrift() = default;

    // 设置评价对象（层间位移角结果）
    void set_inter_story_drift(
        const edp_calculation::InterStoryDriftResult &inter_story_drift)
    {
        inter_story_drift_ = inter_story_drift;
        clear();
    }

    // 安全评价计算
    void TagSafty() override;

    // 获取最大层间位移角结果
    MaxInterStoryDrift get_max_inter_story_drift_result()
    {
        if (!is_tagged_)
        {
            TagSafty();
        }
        return max_inter_story_drift_;
    }

    // 获取所有层的最大层间位移角
    double get_max_inter_story_drift()
    {
        if (!is_tagged_)
        {
            TagSafty();
        }
        return std::max_element(
                   max_inter_story_drift_.abs_max_inter_story_drift_.begin(),
                   max_inter_story_drift_.abs_max_inter_story_drift_.end())
            ->first;
    }

    // 获取所有层的最最大层间位移角信息：出现的时间、楼层和大小
    MaxInterStoryDriftMsg get_max_inter_story_drift_msg();

private:
    // 层间位移角的计算结果
    edp_calculation::InterStoryDriftResult inter_story_drift_;

    // 最大层间位移角结果
    MaxInterStoryDrift max_inter_story_drift_;

    // 是否已经完成了安全评价
    bool is_tagged_{false};

    // 清除计算结果
    void clear()
    {
        max_inter_story_drift_ = MaxInterStoryDrift();
        is_tagged_ = false;
    }
};
} // namespace safty_tagging

#endif // SAFTY_TAGGING_BASED_ON_INTER_STORY_DRIFT_H