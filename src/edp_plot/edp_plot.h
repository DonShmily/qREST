/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\edp_plot\edp_plot.h
** -----
** File Created: Tuesday, 13th August 2024 13:36:40
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Tuesday, 13th August 2024 14:26:30
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 工程需求参数计算结果绘制(Extern "C")

#ifndef EDP_PLOT_H_
#define EDP_PLOT_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    // 建筑信息
    typedef struct
    {
        // 楼层高度
        double *floor_height;
        // 楼层总数
        size_t floor_count;

        // 测点高度
        double *measure_point_height;
        // 测点总数
        size_t measure_point_count;
    } Building_C;

    // 改进的滤波积分方法计算层间位移角
    // @param input_acceleration:
    // 一维数组，尺寸为measure_point_count*time_step_count
    // @param time_step_count: 时间步数
    // @param frequency: 采样频率
    // @param building: 建筑信息
    __declspec(dllexport) void PlotMaxIdr(const double *input_acceleration,
                                          size_t time_step_count,
                                          double frequency,
                                          Building_C *building);

#ifdef __cplusplus
}
#endif

#endif // EDP_PLOT_H_