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
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 基于层间位移角的安全评价方法的实现。

// associated header
#include "based_on_inter_story_drift.h"

// third-party library headers
#include "nlohmann/json.hpp"

// stdc++ headers
#include <fstream>
#include <utility>


namespace safty_tagging
{
// 从配置文件中读取安全评价限值
void BasedOnInterStoryDrift::LoadConfig(const std::string &config_file)
{
    // JSON配置文件
    nlohmann::json config;
    std::ifstream ifs(config_file);
    if (ifs.is_open())
    {
        ifs >> config;
        ifs.close();
    }
    else
    {
        throw std::runtime_error("Cannot open the configuration file.");
    }

    // 读取层间位移角安全评价限值
    safty_tagging_limit_ =
        config["SaftyTagging"]["idr_limits"].get<std::vector<double>>();
}

// 安全评价计算
int BasedOnInterStoryDrift::TagSafty()
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
    all_max_idr_.pos_max_inter_story_drift_.resize(story_number);
    all_max_idr_.neg_max_inter_story_drift_.resize(story_number);
    all_max_idr_.abs_max_inter_story_drift_.resize(story_number);

    // 计算各层最大层间位移角
    for (std::size_t i = 0; i != story_number; ++i)
    {
        all_max_idr_.pos_max_inter_story_drift_.at(i) =
            std::make_pair(pos_max_drift_idx[i].first,
                           pos_max_drift_idx[i].second * 1.0 / freq);
        all_max_idr_.neg_max_inter_story_drift_.at(i) =
            std::make_pair(neg_max_drift_idx[i].first,
                           neg_max_drift_idx[i].second * 1.0 / freq);
        all_max_idr_.abs_max_inter_story_drift_.at(i) =
            std::make_pair(abs_max_drift_idx[i].first,
                           abs_max_drift_idx[i].second * 1.0 / freq);
    }

    // 计算最大层间位移角
    auto max_driift_idx =
        std::max_element(all_max_idr_.abs_max_inter_story_drift_.begin(),
                         all_max_idr_.abs_max_inter_story_drift_.end());
    double time = max_driift_idx->second;
    size_t floor =
        max_driift_idx - all_max_idr_.abs_max_inter_story_drift_.begin();
    double max_idr_res = max_driift_idx->first;
    max_idr_ = std::make_tuple(floor, time, max_idr_res);

    is_tagged_ = true;

    // 根据最大层间位移角进行安全评价
    std::size_t safty_res = 0;
    for (std::size_t i = 0; i != safty_tagging_limit_.size(); ++i)
    {
        if (max_idr_res < safty_tagging_limit_[i])
        {
            safty_res = i;
            break;
        }
    }
    return safty_res;
}
} // namespace safty_tagging