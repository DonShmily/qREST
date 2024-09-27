/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\safty_tagging\basic_safty_tagging.h
** -----
** File Created: Monday, 5th August 2024 17:40:31
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Saturday, 10th August 2024 22:29:38
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 基于工程需求参量安全评价基类。


#ifndef SAFTY_TAGGING_BASIC_SAFTY_TAGGING_H
#define SAFTY_TAGGING_BASIC_SAFTY_TAGGING_H

// stdc++ headers
#include <string>

namespace safty_tagging
{
class BasicSaftyTagging
{
public:
    // 默认构造函数
    BasicSaftyTagging() = default;

    // 析构函数
    virtual ~BasicSaftyTagging() = default;

    // 计算安全评价
    // @return: 安全评价结果，数字越低越安全，0为safe
    virtual int TagSafty() = 0;

    // 获取安全评价结果
    // @return: 安全评价结果，数字越低越安全，0为safe
    virtual int get_tagging_result() = 0;

    // 从配置文件中读取安全评价限值
    // @param config_file 配置文件路径
    virtual void LoadConfig(const std::string &config_file) = 0;

    // 是否已经完成安全评价
    // @return: 是否已经完成安全评价
    bool is_calculated() { return is_tagged_; }

protected:
    // 是否已经完成安全评价
    bool is_tagged_{false};
    // 安全评价结果，数字越低越安全，0为safe
    int safty_tagging_result_{};
};

} // namespace safty_tagging
#endif // SAFTY_TAGGING_BASIC_SAFTY_TAGGING_H