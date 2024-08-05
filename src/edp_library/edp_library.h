#ifndef EDP_LIBRARY_H_
#define EDP_LIBRARY_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        // 层间位移角：(story_count-1)*time_step_count
        double **inter_story_drift;

        // 楼层总数
        size_t story_count;
        // 时间步数
        size_t time_step_count;

    } inter_story_drift_result;

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
    } building;

    // 滤波积分法计算层间位移角
    __declspec(dllexport) inter_story_drift_result *
    FilteringIntegral(const double *const *input_acceleration,
                      size_t time_step_count,
                      double frequency,
                      building *building);

    // 改进的滤波积分方法计算层间位移角
    __declspec(dllexport) inter_story_drift_result *
    ModifiedFilteringIntegral(const double *const *input_acceleration,
                              size_t time_step_count,
                              double frequency,
                              building *building);

    // 释放层间位移角结果内存
    __declspec(dllexport) void
    FreeInterStoryDriftResult(inter_story_drift_result *result);

#ifdef __cplusplus
}
#endif

#endif // EDP_LIBRARY_H_