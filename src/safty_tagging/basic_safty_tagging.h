/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/safty_tagging/basic_safty_tagging.h
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
// Description:
// 基于工程需求参量安全评价基类。


#ifndef SAFTY_TAGGING_BASIC_SAFTY_TAGGING_H
#define SAFTY_TAGGING_BASIC_SAFTY_TAGGING_H

namespace safty_tagging
{

enum class SaftyTaggingResult
{
    Safty,
    Warning,
    Danger
};

class BasicSaftyTagging
{
public:
    // 默认构造函数
    BasicSaftyTagging() = default;

    // 拷贝构造函数
    BasicSaftyTagging(const BasicSaftyTagging &safty_tagging) = default;

    // 移动构造函数
    BasicSaftyTagging(BasicSaftyTagging &&safty_tagging) = default;

    // 析构函数
    virtual ~BasicSaftyTagging() = default;

    // 计算安全评价
    virtual void TagSafty() = 0;


protected:
    // 安全评价结果
    SaftyTaggingResult safty_tagging_result_{};
};


} // namespace safty_tagging

#endif // SAFTY_TAGGING_BASIC_SAFTY_TAGGING_H