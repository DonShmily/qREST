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
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 基于层间位移角的安全评价方法。

#ifndef SAFTY_TAGGING_BASED_ON_INTER_STORY_DRIFT_H
#define SAFTY_TAGGING_BASED_ON_INTER_STORY_DRIFT_H

// stdc++ headers
#include <cstddef>
#include <tuple>
#include <utility>
#include <vector>

// project headers

#include "basic_safty_tagging.h"
#include "edp_calculation/basic_edp_calculation.h"


namespace safty_tagging
{

// 最大层间位移角结果，储存正负向最大层间位移角及对应时间，最大位移及对应时间
struct AllMaxIdr
{
    // 是否需要时间信息
    bool need_time_{false};
    // 各层最大层间位移角结果及对应时间
    std::vector<std::pair<double, double>> pos_max_idr_time_{},
        neg_max_idr_time_{}, abs_max_idr_time_{};
    std::vector<double> pos_max_idr_{}, neg_max_idr_{}, abs_max_idr_{};
};

// 基于层间位移角的安全评价方法
class BasedOnInterStoryDrift : public BasicSaftyTagging
{
public:
    // 默认构造函数
    BasedOnInterStoryDrift() = default;

    // 从层间位移角计算结果构造
    // @param inter_story_drift 层间位移角计算结果
    explicit BasedOnInterStoryDrift(
        const edp_calculation::InterStoryDriftResult &inter_story_drift)
        : inter_story_drift_(inter_story_drift)
    {}

    // 析构函数
    ~BasedOnInterStoryDrift() = default;

    // 设置评价对象（层间位移角结果）
    // @param inter_story_drift 层间位移角计算结果
    void set_inter_story_drift(
        const edp_calculation::InterStoryDriftResult &inter_story_drift)
    {
        inter_story_drift_ = inter_story_drift;
        clear();
    }

    // 设置安全评价限值
    // @param safty_tagging_limit 安全评价限值
    void set_safty_tagging_limit(const std::vector<double> &safty_tagging_limit)
    {
        safty_tagging_limit_ = safty_tagging_limit;
    }

    // 从配置文件中读取安全评价限值
    // @param config_file 配置文件路径
    void LoadConfig(const std::string &config_file = "config/EDP_Config.json");

    // 安全评价计算，
    int TagSafty() override;

    // 获取安全评价结果
    int get_tagging_result() override;

    // 获取最大层间位移角结果
    // @return 所有层的最大层间位移角结果
    AllMaxIdr get_max_inter_story_drift_result();

    // 获取所有层的最大层间位移角
    // @return 最大层间位移角值
    double get_all_max_idr();

    // 获取所有层的最最大层间位移角信息：出现的时间、楼层和大小
    // @return 最大层间位移角信息：楼层索引、时间、层间位移角值
    std::tuple<std::size_t, double, double> get_max_idr();

private:
    // 层间位移角的计算结果
    edp_calculation::InterStoryDriftResult inter_story_drift_;

    // 各层最大层间位移角结果
    AllMaxIdr all_max_idr_;

    // 最大层间位移角结果，楼层索引、时间、层间位移角值
    std::tuple<std::size_t, double, double> max_idr_{};

    // 是否已经完成了安全评价
    bool is_tagged_{false};

    // 安全评价的限值
    std::vector<double> safty_tagging_limit_{};

    // 清除计算结果
    void clear()
    {
        all_max_idr_ = AllMaxIdr();
        is_tagged_ = false;
    }
};
} // namespace safty_tagging

#endif // SAFTY_TAGGING_BASED_ON_INTER_STORY_DRIFT_H