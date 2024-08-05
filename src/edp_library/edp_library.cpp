// associated headers
#include "edp_library.h"

// stdc++ headers
#include <new>
#include <vector>

// project headers
#include "data_structure/acceleration.h"
#include "data_structure/building.h"
#include "edp_calculation/basic_edp_calculation.h"
#include "edp_calculation/filtering_integral.h"
#include "edp_calculation/modified_filtering_integral.h"

// 滤波积分方法计算层间位移角
inter_story_drift_result *
FilteringIntegral(const double *const *input_acceleration,
                  size_t time_step_count,
                  double frequency,
                  building *building)
{
    /// 创建加速度计算对象
    auto acceleration = data_structure::Acceleration(
        std::vector<std::vector<double>>(building->measure_point_count,
                                         std::vector<double>(time_step_count)),
        frequency,
        0.01);
    for (std::size_t i = 0; i < building->measure_point_count; ++i)
    {
        acceleration.data()[i].assign(input_acceleration[i],
                                      input_acceleration[i] + time_step_count);
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
    auto calculate_result = filt_integral.get_filtering_interp_result();

    // 创建输出对象
    inter_story_drift_result *idr_result = new inter_story_drift_result;
    idr_result->inter_story_drift = new double *[building->floor_count - 1];
    idr_result->story_count = building->floor_count - 1;
    idr_result->time_step_count = time_step_count;
    for (size_t i = 0; i != idr_result->story_count; ++i)
    {
        idr_result->inter_story_drift[i] = new double[time_step_count];
        std::copy(calculate_result.get_inter_story_drift().col(i).begin(),
                  calculate_result.get_inter_story_drift().col(i).end(),
                  idr_result->inter_story_drift[i]);
    }

    return idr_result;
}

// 改进的滤波积分方法计算层间位移角
inter_story_drift_result *
ModifiedFilteringIntegral(const double *const *input_acceleration,
                          size_t time_step_count,
                          double frequency,
                          building *building)
{
    // 创建加速度计算对象
    auto acceleration = data_structure::Acceleration(
        std::vector<std::vector<double>>(building->measure_point_count,
                                         std::vector<double>(time_step_count)),
        frequency,
        0.01);
    for (std::size_t i = 0; i < building->measure_point_count; ++i)
    {
        acceleration.data()[i].assign(input_acceleration[i],
                                      input_acceleration[i] + time_step_count);
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
    auto calculate_result = m_filter_integral.get_filtering_interp_result();

    // 创建输出对象
    inter_story_drift_result *idr_result = new inter_story_drift_result;
    idr_result->inter_story_drift = new double *[building->floor_count - 1];
    idr_result->story_count = building->floor_count - 1;
    idr_result->time_step_count = time_step_count;
    for (size_t i = 0; i != idr_result->story_count; ++i)
    {
        idr_result->inter_story_drift[i] = new double[time_step_count];
        std::copy(calculate_result.get_inter_story_drift().col(i).begin(),
                  calculate_result.get_inter_story_drift().col(i).end(),
                  idr_result->inter_story_drift[i]);
    }

    return idr_result;
}

// 释放层间位移角结果内存
void FreeInterStoryDriftResult(inter_story_drift_result *result)
{
    if (result)
    {
        for (size_t i = 0; i != result->story_count; ++i)
        {
            delete[] result->inter_story_drift[i];
        }
        delete[] result->inter_story_drift;
        delete result;
    }
}