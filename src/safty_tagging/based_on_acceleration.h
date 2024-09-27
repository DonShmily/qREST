/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\safty_tagging\based_on_acceleration.h
** -----
** File Created: Thursday, 26th September 2024 22:54:29
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Thursday, 26th September 2024 22:55:41
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description: 基于加速度的安全评价方法。

#ifndef SAFTY_TAGGING_BASED_ON_ACCELERATION_H
#define SAFTY_TAGGING_BASED_ON_ACCELERATION_H

// stdc++ headers
#include <memory>

// project headers
#include "basic_safty_tagging.h"
#include "edp_calculation/basic_edp_calculation.h"


namespace safty_tagging
{

// 最大加速度结果，储存正负向最大加速度及对应时间
struct AllMaxAcc
{
    // 是否需要时间信息
    bool need_time_{false};
    // 各层最加速度结果及对应时间
    std::vector<std::pair<double, double>> pos_max_acc_time_{},
        neg_max_acc_time_{}, abs_max_acc_time_{};
    std::vector<double> pos_max_acc_{}, neg_max_acc_{}, abs_max_acc_{};
};

class BasedOnAcceleration : public BasicSaftyTagging
{
public:
    // 默认构造函数
    BasedOnAcceleration() = default;

    // 从工程需求参量计算结果构造
    // @param edp_result 工程需求参量计算结果
    explicit BasedOnAcceleration(
        std::shared_ptr<edp_calculation::EdpResult> edp_result)
        : edp_result_(edp_result)
    {}

    // 从配置文件中读取安全评价限值
    // @param config_file 配置文件路径
    void LoadConfig(
        const std::string &config_file = "config/EDP_Config.json") override;

    // 计算安全评价，返回安全评价结果
    // @return: 安全评价结果，数字越低越安全，0为safe
    int TagSafty() override;

    // 获取安全评价结果
    // @return: 安全评价结果，数字越低越安全，0为safe
    int get_tagging_result() override;

    // 获取所有层的最大加速度结果
    // @return 所有层的最大加速度结果
    AllMaxAcc get_max_acc_result();

    // 获取所有层的最大加速度
    // @return 最大加速度值
    double get_all_max_acc();

    // 获取所有层的最最大加速度信息：出现的时间、楼层和大小
    // @return 最大加速度信息：楼层索引、时间、层间位移角值
    std::tuple<std::size_t, double, double> get_max_acc();

private:
    // 工程需求参量计算结果，本类只是用加速度部分
    std::shared_ptr<edp_calculation::EdpResult> edp_result_;

    // 各层最大加速度结果
    AllMaxAcc all_max_acc_;

    // 最大楼层加速度结果，楼层索引、时间、最大加速度值
    std::tuple<std::size_t, double, double> max_acc_{};

    // 安全评价的限值
    std::vector<double> safty_tagging_limit_{};

    // 清除计算结果
    void clear()
    {
        all_max_acc_ = AllMaxAcc();
        is_tagged_ = false;
    }
};

} // namespace safty_tagging

#endif // SAFTY_TAGGING_BASED_ON_ACCELERATION_H