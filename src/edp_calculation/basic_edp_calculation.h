/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/edp_calculation/basic_edp_calculation.h
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
// Description:
// 工程需求参量计算基类。
// 每种方法的类都应当对应唯一的计算结果类，结果类和计算方法类定义在同个头文件中。

#ifndef EDP_CALCULATION_BASIC_EDP_CALCULATION_H
#define EDP_CALCULATION_BASIC_EDP_CALCULATION_H

// stdc++ headers
#include <memory>

// project headers
#include "data_structure/acceleration.h"
#include "data_structure/building.h"


namespace edp_calculation
{

// 工程需求参量计算基类，计算方法基类至少接收一个加速度数据
class BasicEdpCalculation
{
public:
    // 默认构造函数
    BasicEdpCalculation() = default;

    // 从加速度数据中构造
    // @param acceleration 加速度数据
    explicit BasicEdpCalculation(
        const data_structure::Acceleration &acceleration,
        const data_structure::Building &building)
        : input_acceleration_(acceleration), building_(building)
    {}

    // 从加速度数据指针中构造
    // @param acceleration_ptr 加速度数据指针
    explicit BasicEdpCalculation(
        std::shared_ptr<const data_structure::Acceleration> acceleration_ptr,
        std::shared_ptr<data_structure::Building> building_ptr)
        : input_acceleration_(*acceleration_ptr), building_(*building_ptr)
    {}

    // 拷贝构造函数
    BasicEdpCalculation(const BasicEdpCalculation &other) = default;

    // 移动构造函数
    BasicEdpCalculation(BasicEdpCalculation &&other) = default;

    // 析构函数
    virtual ~BasicEdpCalculation() = default;

    // 计算工程需求参量函数入口
    virtual void CalculateEdp() = 0;

protected:
    // 单方向加速度数据指针
    const data_structure::Acceleration input_acceleration_{};
    // 建筑信息的指针
    data_structure::Building building_{};
};

} // namespace edp_calculation

#endif // EDP_CALCULATION_BASIC_EDP_CALCULATION_H