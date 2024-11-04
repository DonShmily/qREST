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
** Last Modified: Thursday, 26th September 2024 22:53:05
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 基于层间位移角的安全评价方法的实现。

// associated header
#include "based_on_inter_story_drift.h"

// stdc++ headers
#include <utility>


namespace safty_tagging
{
// 从配置文件中读取安全评价限值
void BasedOnInterStoryDrift::LoadConfig(
    std::shared_ptr<settings::Config> config)
{
    // 读取配置文件
    safty_tagging_limit_ = config->safty_idr_threshold;
}

// 获取安全评价结果
int BasedOnInterStoryDrift::get_tagging_result()
{
    if (!is_tagged_)
    {
        TagSafty();
    }
    return safty_tagging_result_;
}

// 获取最大层间位移角结果
AllMaxIdr BasedOnInterStoryDrift::get_max_inter_story_drift_result()
{
    {
        if (!is_tagged_)
        {
            TagSafty();
        }
        return all_max_idr_;
    }
}

// 获取所有层的最大层间位移角
double BasedOnInterStoryDrift::get_all_max_idr()
{
    if (!is_tagged_)
    {
        TagSafty();
    }
    return *std::max_element(all_max_idr_.abs_max_idr_.begin(),
                             all_max_idr_.abs_max_idr_.end());
}

// 获取所有层的最最大层间位移角信息：出现的时间、楼层和大小
std::tuple<std::size_t, double, double> BasedOnInterStoryDrift::get_max_idr()
{
    if (!is_tagged_)
    {
        TagSafty();
    }
    return max_idr_;
}

// 安全评价计算
int BasedOnInterStoryDrift::TagSafty()
{
    // 获取有层间位移角的层数
    size_t story_number = edp_result_->get_inter_story_drift().get_col_number();
    // 获取采样频率
    double freq = edp_result_->get_displacement().get_frequency();

    // 获取最大层间位移角信息（索引）
    auto pos_max_drift_idx = edp_result_->get_inter_story_drift().PositiveMax(),
         neg_max_drift_idx = edp_result_->get_inter_story_drift().NegativeMax(),
         abs_max_drift_idx = edp_result_->get_inter_story_drift().AbsoluteMax();
    // 初始化最大层间位移角结果
    all_max_idr_.pos_max_idr_time_.resize(story_number);
    all_max_idr_.neg_max_idr_time_.resize(story_number);
    all_max_idr_.abs_max_idr_time_.resize(story_number);

    // 计算各层最大层间位移角
    for (std::size_t i = 0; i != story_number; ++i)
    {
        if (all_max_idr_.need_time_)
        {
            all_max_idr_.pos_max_idr_time_.at(i) =
                std::make_pair(pos_max_drift_idx[i].first,
                               pos_max_drift_idx[i].second * 1.0 / freq);
            all_max_idr_.neg_max_idr_time_.at(i) =
                std::make_pair(neg_max_drift_idx[i].first,
                               neg_max_drift_idx[i].second * 1.0 / freq);
            all_max_idr_.abs_max_idr_time_.at(i) =
                std::make_pair(abs_max_drift_idx[i].first,
                               abs_max_drift_idx[i].second * 1.0 / freq);
        }
        all_max_idr_.pos_max_idr_.push_back(pos_max_drift_idx[i].first);
        all_max_idr_.neg_max_idr_.push_back(neg_max_drift_idx[i].first);
        all_max_idr_.abs_max_idr_.push_back(abs_max_drift_idx[i].first);
    }

    // 计算最大层间位移角
    auto max_driift_idx = std::max_element(all_max_idr_.abs_max_idr_.begin(),
                                           all_max_idr_.abs_max_idr_.end());
    size_t floor = max_driift_idx - all_max_idr_.abs_max_idr_.begin();

    if (all_max_idr_.need_time_)
    {
        double time = all_max_idr_.abs_max_idr_time_.at(floor).second;
        max_idr_ = std::make_tuple(floor, time, *max_driift_idx);
    }

    // 根据最大层间位移角进行安全评价
    std::size_t safty_res = 0;
    for (std::size_t i = 0; i != safty_tagging_limit_.size(); ++i)
    {
        if (*max_driift_idx < safty_tagging_limit_[i])
        {
            safty_res = i;
            break;
        }
    }

    // 完成安全评价
    is_tagged_ = true;
    return safty_res;
}
} // namespace safty_tagging