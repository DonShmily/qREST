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
// 基于工程需求参量安全评价基类和计算结果基类，每种方法的类都应当对应唯一的计算结果类。


#ifndef SAFTY_TAGGING_BASIC_SAFTY_TAGGING_H
#define SAFTY_TAGGING_BASIC_SAFTY_TAGGING_H

// stdc++ headers

// project headers

namespace safty_tagging
{
class BasicSaftyTaggingResult
{
public:
    // 默认构造函数
    BasicSaftyTaggingResult() = default;
    // 拷贝构造函数
    BasicSaftyTaggingResult(const BasicSaftyTaggingResult &other) = default;
    // 移动构造函数
    BasicSaftyTaggingResult(BasicSaftyTaggingResult &&other) = default;
    // 析构函数
    virtual ~BasicSaftyTaggingResult() = default;

private:
    // 计算结果，具体内容由方法决定
};

class BasicSaftyTagging
{
public:
};


} // namespace safty_tagging

#endif // SAFTY_TAGGING_BASIC_SAFTY_TAGGING_H