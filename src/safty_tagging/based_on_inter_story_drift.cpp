/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\safty_tagging\based_on_inter_story_drift.cpp
** -----
** File Created: Monday, 5th August 2024 17:40:31
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Saturday, 10th August 2024 22:29:21
** Modified By: Dong Feiyue (donfeiyue@outlook.com>)
*/

// Description:
// 基于层间位移角的安全评价方法的实现。

// associated header
#include "based_on_inter_story_drift.h"

// stdc++ headers
#include <utility>


namespace safty_tagging
{
// 安全评价计算
void BasedOnInterStoryDrift::TagSafty()
{
    // 获取有层间位移角的层数
    size_t story_number =
        inter_story_drift_.get_inter_story_drift().get_col_number();
    // 获取采样频率
    double freq = inter_story_drift_.get_displacement().get_frequency();
    // 获取最大层间位移角信息（索引）
    auto pos_max_drift_idx =
             inter_story_drift_.get_inter_story_drift().PositiveMax(),
         neg_max_drift_idx =
             inter_story_drift_.get_inter_story_drift().NegativeMax(),
         abs_max_drift_idx =
             inter_story_drift_.get_inter_story_drift().AbsoluteMax();
    // 初始化最大层间位移角结果
    max_inter_story_drift_.pos_max_inter_story_drift_.resize(story_number);
    max_inter_story_drift_.neg_max_inter_story_drift_.resize(story_number);
    max_inter_story_drift_.abs_max_inter_story_drift_.resize(story_number);
    // 计算最大层间位移角
    for (std::size_t i = 0; i != story_number; ++i)
    {
        max_inter_story_drift_.pos_max_inter_story_drift_.at(i) =
            std::make_pair(pos_max_drift_idx[i].first,
                           pos_max_drift_idx[i].second * 1.0 / freq);
        max_inter_story_drift_.neg_max_inter_story_drift_.at(i) =
            std::make_pair(neg_max_drift_idx[i].first,
                           neg_max_drift_idx[i].second * 1.0 / freq);
        max_inter_story_drift_.abs_max_inter_story_drift_.at(i) =
            std::make_pair(abs_max_drift_idx[i].first,
                           abs_max_drift_idx[i].second * 1.0 / freq);
    }
    is_tagged_ = true;
}

// 获取所有层的最最大层间位移角信息：出现的时间、楼层和大小
MaxInterStoryDriftMsg BasedOnInterStoryDrift::get_max_inter_story_drift_msg()
{
    if (!is_tagged_)
    {
        TagSafty();
    }
    auto max_driift_idx = std::max_element(
        max_inter_story_drift_.abs_max_inter_story_drift_.begin(),
        max_inter_story_drift_.abs_max_inter_story_drift_.end());
    double time = max_driift_idx->second;
    size_t floor = max_driift_idx
                   - max_inter_story_drift_.abs_max_inter_story_drift_.begin();
    double inter_story_drift = max_driift_idx->first;
    return {time, floor, inter_story_drift};
}
} // namespace safty_tagging