/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\edp_library\edp_library.cpp
** -----
** File Created: Monday, 5th August 2024 17:40:31
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Monday, 12th August 2024 12:17:17
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 工程需求参量计算到出库(Extern "C")的实现

// associated headers
#include "edp_library.h"

// stdc++ headers
#include <algorithm>
#include <cstddef>
#include <new>
#include <vector>

// project headers
#include "data_structure/acceleration.h"
#include "data_structure/building.h"
#include "edp_calculation/basic_edp_calculation.h"
#include "edp_calculation/filtering_integral.h"
#include "edp_calculation/modified_filtering_integral.h"

// 滤波积分方法计算层间位移角
Idr *FilteringIntegral(const double *input_acceleration,
                       size_t time_step_count,
                       double frequency,
                       Building *building)
{
    /// 创建加速度计算对象
    auto acceleration = data_structure::Acceleration(
        std::vector<std::vector<double>>(building->measure_point_count,
                                         std::vector<double>(time_step_count)),
        frequency,
        0.01);
    for (std::size_t i = 0; i < building->measure_point_count; ++i)
    {
        acceleration.data()[i].assign(input_acceleration + i * time_step_count,
                                      input_acceleration
                                          + (i + 1) * time_step_count);
    }

    // 创建建筑信息计算对象
    auto building_obj = data_structure::Building(
        std::vector<double>(building->measure_point_height,
                            building->measure_point_height
                                + building->measure_point_count),
        std::vector<double>(building->floor_height,
                            building->floor_height + building->floor_count));

    // 创建滤波积分方法计算对象
    edp_calculation::FilteringIntegral filt_integral(
        acceleration, building_obj, 2, 0.1, 20);
    filt_integral.CalculateEdp();
    auto &calculate_result = filt_integral.get_filtering_interp_result();

    // 创建输出对象
    Idr *idr_res = new Idr;
    idr_res->story_count = building->floor_count - 1;
    idr_res->time_step_count = time_step_count;
    idr_res->idr = new double[idr_res->story_count * (time_step_count)];
    for (size_t i = 0; i != idr_res->story_count; ++i)
    {
        std::copy(calculate_result.get_inter_story_drift().col(i).begin(),
                  calculate_result.get_inter_story_drift().col(i).end(),
                  idr_res->idr + i * time_step_count);
    }

    return idr_res;
}

// 改进的滤波积分方法计算层间位移角
Idr *ModifiedFilteringIntegral(const double *input_acceleration,
                               size_t time_step_count,
                               double frequency,
                               Building *building)
{
    // 创建加速度计算对象
    auto acceleration = data_structure::Acceleration(
        std::vector<std::vector<double>>(building->measure_point_count,
                                         std::vector<double>(time_step_count)),
        frequency,
        0.01);
    for (std::size_t i = 0; i < building->measure_point_count; ++i)
    {
        acceleration.data()[i].assign(input_acceleration + i * time_step_count,
                                      input_acceleration
                                          + (i + 1) * time_step_count);
    }

    // 创建建筑信息计算对象
    auto building_obj = data_structure::Building(
        std::vector<double>(building->measure_point_height,
                            building->measure_point_height
                                + building->measure_point_count),
        std::vector<double>(building->floor_height,
                            building->floor_height + building->floor_count));

    // 创建计算对象
    auto m_filter_integral = edp_calculation::ModifiedFilteringIntegral(
        acceleration, building_obj, 2);
    m_filter_integral.CalculateEdp();
    auto &calculate_result = m_filter_integral.get_filtering_interp_result();

    // 创建输出对象
    Idr *idr_res = new Idr;
    idr_res->story_count = building->floor_count - 1;
    idr_res->time_step_count = time_step_count;
    idr_res->idr = new double[idr_res->story_count * time_step_count];
    for (size_t i = 0; i != idr_res->story_count; ++i)
    {
        std::copy(calculate_result.get_inter_story_drift().col(i).begin(),
                  calculate_result.get_inter_story_drift().col(i).end(),
                  idr_res->idr + i * time_step_count);
    }

    return idr_res;
}

// 释放层间位移角结果内存
void FreeIdr(Idr *memory)
{
    if (memory != NULL)
    {
        delete[] memory->idr;
        delete memory;
    }
}

// 计算最大层间位移角
MaxIdr *GetMaxIdr(const Idr *idr_result, double frequency)
{
    // 创建输出对象
    MaxIdr *max_idr = new MaxIdr;
    max_idr->max_idr = new double[idr_result->story_count];
    max_idr->max_idr_time = new double[idr_result->story_count];
    max_idr->story_count = idr_result->story_count;

    // 计算最大层间位移角
    for (std::size_t i = 0; i < idr_result->story_count; ++i)
    {
        auto beg = idr_result->idr + i * idr_result->time_step_count,
             end = idr_result->idr + (i + 1) * idr_result->time_step_count;
        auto max_idx = std::max_element(beg, end, [](double a, double b) {
            return std::abs(a) < std::abs(b);
        });
        max_idr->max_idr[i] = std::abs(*max_idx);
        auto l = std::distance(beg, max_idx);
        max_idr->max_idr_time[i] = l * 1.0 / frequency;
    }
    max_idr->max_idr_story = std::distance(
        max_idr->max_idr,
        std::max_element(max_idr->max_idr,
                         max_idr->max_idr + max_idr->story_count));

    return max_idr;
}

// 释放最大层间位移角结果内存
void FreeMaxIdr(MaxIdr *memory)
{
    if (memory != NULL)
    {
        delete[] memory->max_idr;
        delete[] memory->max_idr_time;
        delete memory;
    }
}