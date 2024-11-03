/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\edp_library\edp_library.h
** -----
** File Created: Monday, 5th August 2024 17:40:31
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Monday, 12th August 2024 12:17:00
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 工程需求参量计算到出库(Extern "C")

#ifndef EDP_LIBRARY_H_
#define EDP_LIBRARY_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    // 层间位移角时程
    typedef struct
    {
        // 层间位移角：story_count*time_step_count
        double *idr;

        // 楼层总数story_count = floor_count - 1
        size_t story_count;
        // 时间步数
        size_t time_step_count;

    } Idr;

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
    } Building;

    // 滤波积分法计算层间位移角
    // @param input_acceleration:
    // 一维数组，尺寸为measure_point_count*time_step_count
    // @param time_step_count: 时间步数
    // @param frequency: 采样频率
    // @param building: 建筑信息
    // @return Idr: 层间位移角
    __declspec(dllexport) Idr *
    FilteringIntegral(const double *input_acceleration,
                      size_t time_step_count,
                      double frequency,
                      Building *building);

    // 改进的滤波积分方法计算层间位移角
    // @param input_acceleration:
    // 一维数组，尺寸为measure_point_count*time_step_count
    // @param time_step_count: 时间步数
    // @param frequency: 采样频率
    // @param building: 建筑信息
    // @return Idr: 层间位移角
    __declspec(dllexport) Idr *
    ModifiedFilteringIntegral(const double *input_acceleration,
                              size_t time_step_count,
                              double frequency,
                              Building *building);

    // 释放层间位移角结果内存
    // @param result: 层间位移角
    __declspec(dllexport) void FreeIdr(Idr *memory);

    // 最大层间位移角
    typedef struct
    {
        // 各最大层间位移角：story_count
        double *max_idr;

        // 各层最大层间位移角对应的时间(s)
        double *max_idr_time;

        // 最大层间位移角对应的楼层
        size_t max_idr_story;

        // 楼层总数
        size_t story_count;

    } MaxIdr;

    // 计算最大层间位移角
    __declspec(dllexport) MaxIdr *GetMaxIdr(const Idr *idr_result,
                                            double frequency);

    // 释放最大层间位移角结果内存
    __declspec(dllexport) void FreeMaxIdr(MaxIdr *memory);

#ifdef __cplusplus
}
#endif

#endif // EDP_LIBRARY_H_