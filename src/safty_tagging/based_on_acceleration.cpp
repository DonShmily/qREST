/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\safty_tagging\ba.cpp
** -----
** File Created: Thursday, 26th September 2024 23:51:11
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Thursday, 26th September 2024 23:51:17
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description: 基于加速度的安全评价方法实现。

// associated header
#include "based_on_acceleration.h"

// stdc++ headers
#include <cstddef>

namespace safty_tagging
{
// 从配置文件中读取安全评价限值
void BasedOnAcceleration::LoadConfig(std::shared_ptr<settings::Config> config)
{
    // 读取配置文件
    safty_tagging_limit_ = config->safty_acc_threshold;
}

// 获取安全评价结果
int BasedOnAcceleration::get_tagging_result()
{
    if (!is_tagged_)
    {
        TagSafty();
    }
    return safty_tagging_result_;
}

// 获取所有层的最大加速度结果
AllMaxAcc BasedOnAcceleration::get_max_acc_result()
{
    if (!is_tagged_)
    {
        TagSafty();
    }
    return all_max_acc_;
}

// 获取所有层的最大加速度
double BasedOnAcceleration::get_all_max_acc()
{
    if (!is_tagged_)
    {
        TagSafty();
    }
    return *std::max_element(all_max_acc_.abs_max_acc_.begin(),
                             all_max_acc_.abs_max_acc_.end());
}

// 获取所有层的最最大加速度信息：出现的时间、楼层和大小
std::tuple<std::size_t, double, double> BasedOnAcceleration::get_max_acc()
{
    if (!is_tagged_)
    {
        TagSafty();
    }
    return max_acc_;
}

// 计算安全评价，返回安全评价结果
int BasedOnAcceleration::TagSafty()
{
    // 获取层数
    size_t story_number = edp_result_->get_acceleration().get_col_number();
    // 获取采样频率
    double freq = edp_result_->get_acceleration().get_frequency();

    // 获取最大加速度信息
    auto pos_max_acc = edp_result_->get_acceleration().PositiveMax(),
         neg_max_acc = edp_result_->get_acceleration().NegativeMax(),
         abs_max_acc = edp_result_->get_acceleration().AbsoluteMax();

    // 计算最大加速度信息
    for (size_t i = 0; i < story_number; ++i)
    {
        if (all_max_acc_.need_time_)
        {
            // 获取带时间的最大加速度
        }
        all_max_acc_.abs_max_acc_.push_back(pos_max_acc[i].first);
        all_max_acc_.pos_max_acc_.push_back(pos_max_acc[i].second);
        all_max_acc_.neg_max_acc_.push_back(neg_max_acc[i].second);
    }

    // 计算安全评价
    is_tagged_ = true;
    int safty_res = 0;

    return safty_res;
}


} // namespace safty_tagging