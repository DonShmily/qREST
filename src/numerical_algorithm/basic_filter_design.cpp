/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/numerical_algorithm/basic_filter_design.cpp
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
// Description:
// 数值算法：滤波器设计基类的实现

// associated header
#include "basic_filter_design.h"

// stdc++ headers
#include <vector>

namespace numerical_algorithm
{

// 带通滤波器构造函数
BasicFilterDesign::BasicFilterDesign(int filter_order,
                                     double low_frequency,
                                     double high_frequency,
                                     FilterType filter_type)
    : filter_order_(filter_order),
      low_frequency_(low_frequency),
      high_frequency_(high_frequency),
      filter_type_(filter_type)
{}

// 低通/高通滤波器构造函数（默认低通）
BasicFilterDesign::BasicFilterDesign(int filter_order,
                                     double frequency,
                                     FilterType filter_type)
    : filter_order_(filter_order), filter_type_(filter_type)
{
    if (filter_type == FilterType::lowpass)
    {
        low_frequency_ = frequency;
        high_frequency_ = 0.0;
    }
    else if (filter_type == FilterType::highpass)
    {
        low_frequency_ = 0.0;
        high_frequency_ = frequency;
    }
}

} // namespace numerical_algorithm
