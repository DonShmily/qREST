/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/edp_calculation/filtering_interp.cpp
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
// Description:
// 工程需求参量计算方法：滤波积分插值法的实现

// associated header
#include "filtering_integral.h"

// stdc++ headers
#include <cmath>
#include <cstddef>
#include <memory>

// third-party library headers

// project headers
#include <data_structure/story_drift.h>
#include "numerical_algorithm/basic_filtering.h"
#include "numerical_algorithm/butterworth_filter_design.h"
#include "numerical_algorithm/filter.h"
#include "numerical_algorithm/filtfilt.h"
#include "numerical_algorithm/integral.h"
#include "numerical_algorithm/interp.h"
#include "numerical_algorithm/vector_calculation.h"


namespace edp_calculation
{
void FilteringIntegral::CalculateEdp()
{
    // 1.确定计算参数
    // 1.1确定滤波生成器
    auto filter_generator = numerical_algorithm::ButterworthFilterDesign(
        filter_order_, low_frequency_, high_frequency_, method_.filter_type_);

    // 1.2确定滤波函数
    std::shared_ptr<numerical_algorithm::BasicFiltering> filter_function =
        nullptr;
    switch (method_.filter_function_)
    {
        case numerical_algorithm::FilterFunction::filtfilt:
            filter_function = std::make_shared<numerical_algorithm::FiltFilt>(
                filter_generator);
            break;
        case numerical_algorithm::FilterFunction::filter:
            filter_function =
                std::make_shared<numerical_algorithm::Filter>(filter_generator);
            break;
        default:
            filter_function = std::make_shared<numerical_algorithm::FiltFilt>(
                filter_generator);
            break;
    }

    // 1.3确定插值方法
    numerical_algorithm::Interp interp_function(method_.interp_type_);

    // 2.滤波积分插值计算层间位移角
    double dt = input_acceleration_ptr_->get_time_step();
    // 2.1 加速度滤波
    auto filtered_acceleration =
        filter_function->Filtering(input_acceleration_ptr_->get_data());
    //  2.2 加速度积分到速度
    auto velocity = numerical_algorithm::Cumtrapz(filtered_acceleration, dt);
    // 2.3 速度滤波
    auto interp_velocity = filter_function->Filtering(velocity);
    // 2.4 速度积分到位移
    auto displacement = numerical_algorithm::Cumtrapz(interp_velocity, dt);
    // 2.5 位移滤波
    auto filtered_displacement = filter_function->Filtering(displacement);
    // 2.6 位移插值
    result_ptr_->displacement_ptr_ =
        std::make_shared<data_structure::Displacement>(
            std::vector<std::vector<double>>(),
            input_acceleration_ptr_->get_frequency());
    result_ptr_->displacement_ptr_->data() =
        interp_function.Interpolation(building_ptr_->get_measuren_height(),
                                      filtered_displacement,
                                      building_ptr_->get_floor_height());
    // 2.7 计算层间位移
    auto interstory_displacement =
        result_ptr_->displacement_ptr_->interstory_displacement();
    result_ptr_->story_drift_ptr_ =
        std::make_shared<data_structure::StoryDrift>();
    auto interstory_height = building_ptr_->get_inter_height();
    for (std::size_t i = 0; i < interstory_displacement.data().size(); ++i)
    {
        result_ptr_->story_drift_ptr_->data().push_back(
            numerical_algorithm::VectorOperation(
                interstory_displacement.data()[i], interstory_height[i], '/'));
    }
}
} // namespace edp_calculation