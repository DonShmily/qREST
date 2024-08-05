#ifndef EDP_LIBRARY_H_
#define EDP_LIBRARY_H_

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        double **inter_story_drift;

        size_t story_count;

        size_t time_step_count;
    } inter_story_drift_result;

    typedef struct
    {
        double *floor_height;

        size_t floor_count;

        double *measure_point_height;

        size_t measure_point_count;
    } building;

    // 滤波积分法计算层间位移角
    __declspec(dllexport) inter_story_drift_result *FilteringIntegral(
        const double **input_acceleration,
        const size_t time_step_count,
        double frequency,
        const building *building);

    // 改进的滤波积分方法计算层间位移角
    __declspec(dllexport) inter_story_drift_result *ModifiedFilteringIntegral(
        const double **input_acceleration,
        const size_t time_step_count,
        double frequency,
        const building *building);

    // 释放层间位移角结果内存
    __declspec(dllexport) void FreeInterStoryDriftResult(
        inter_story_drift_result *result);

#ifdef __cplusplus
}
#endif

#endif // EDP_LIBRARY_H_