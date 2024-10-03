/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\parameters_identification\simple_parameters_identification.h
** -----
** File Created: Monday, 30th September 2024 16:45:25
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Monday, 30th September 2024 16:46:04
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:简单参数识别类，使用顶层的加速度时程识别周期和阻尼比。
// 具体方法为频谱峰值和半功率带宽法。

#ifndef PARAMETERS_IDENTIFICATION_SIMPLE_PARAMETERS_IDENTIFICATION_H
#define PARAMETERS_IDENTIFICATION_SIMPLE_PARAMETERS_IDENTIFICATION_H

// stdc++ headers
#include <vector>

// project headers
#include "basic_parameters_identification.h"
#include "data_structure/acceleration.h"
#include "data_structure/building.h"
#include "numerical_algorithm/math_function.h"

namespace parameters_identification
{
// 简单参数识别类
class SimpleParametersIdentification : public BasicParametersIdentification
{
public:
    // 默认构造函数
    SimpleParametersIdentification() = default;

    // 从加速度数据和建筑信息指针构造
    // @param acceleration 加速度数据
    // @param building 建筑结构信息指针
    SimpleParametersIdentification(
        const data_structure::Acceleration &acceleration,
        const std::shared_ptr<data_structure::Building> &building)
        : BasicParametersIdentification(acceleration, building)
    {}

    // 从加速度数据和建筑信息构造
    // @param acceleration 加速度数据
    // @param building 建筑结构信息
    SimpleParametersIdentification(
        const data_structure::Acceleration &acceleration,
        const data_structure::Building &building)
        : BasicParametersIdentification(acceleration, building)
    {}

    // 识别结构的固有周期和阻尼比
    void Identify() override;

private:
};
} // namespace parameters_identification

#endif // PARAMETERS_IDENTIFICATION_SIMPLE_PARAMETERS_IDENTIFICATION_H