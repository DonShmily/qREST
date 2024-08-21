/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\edp_plot\edp_plot.cpp
** -----
** File Created: Tuesday, 13th August 2024 13:36:49
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Tuesday, 13th August 2024 15:33:42
** Modified By: Dong Feiyue (donfeiyue@outlook.com>)
*/

// Description:
// 工程需求参数计算结果绘制(Extern "C")的实现

// associated header
#include "edp_plot.h"

// stdc++ headers
#include <vector>

// project headers
#include "data_structure/acceleration.h"
#include "data_structure/building.h"
#include "data_visualization/plotting_xy.h"
#include "edp_calculation/modified_filtering_integral.h"


void PlotMaxIdr(const double *input_acceleration,
                size_t time_step_count,
                double frequency,
                Building_C *building)
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

    // 获取最大层间位移角
    std::vector<double> max_idr;
    for (std::size_t i = 0;
         i != calculate_result.get_inter_story_drift().get_col_number();
         ++i)
    {
        max_idr.push_back(std::abs(*std::max_element(
            calculate_result.get_inter_story_drift().col(i).begin(),
            calculate_result.get_inter_story_drift().col(i).end(),
            [](double a, double b) { return std::abs(a) < std::abs(b); })));
    }

    // 绘制图形
    auto floor = building_obj.get_floor_height();
    data_visualization::PlotXY plot(
        max_idr,
        std::vector<double>(floor.begin() + 1, floor.end()),
        "Max IDR",
        "IDR",
        "Floor Height(m)");
    plot.set_type(data_visualization::PlotType::Step);
    plot.set_size(800, 1600);
    plot.set_zoom({0, 0.1, 1, 0.85});
    plot.set_ranges({0,
                     *std::max_element(max_idr.begin(), max_idr.end()),
                     floor[1],
                     floor.back(),
                     0,
                     0});
    plot.DrawFile("output/MaxIDR");
}