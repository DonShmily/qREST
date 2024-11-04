/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\edp_calculation\filtering_integral.cpp
** -----
** File Created: Sunday, 14th July 2024 21:20:23
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Monday, 4th November 2024 00:34:51
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 工程需求参量计算方法：滤波积分插值法的实现

// associated header
#include "filtering_integral.h"

// stdc++ headers
#include <memory>
#include <vector>

// project headers
#include "numerical_algorithm/basic_filtering.h"
#include "numerical_algorithm/butterworth_filter_design.h"
#include "numerical_algorithm/filter.h"
#include "numerical_algorithm/filtfilt.h"
#include "numerical_algorithm/integral.h"
#include "numerical_algorithm/interp.h"
#include "numerical_algorithm/vector_calculation.h"


namespace edp_calculation
{
// 滤波积分插值法计算的入口
void FilteringIntegral::CalculateEdp()
{
    // 1.确定计算参数
    // 1.1确定滤波生成器
    double low = parameter_.low_frequency_ / input_acceleration_.get_frequency()
                 * 2,
           high = parameter_.high_frequency_ / parameter_.low_frequency_ * low;
    auto filter_generator = numerical_algorithm::ButterworthFilterDesign(
        parameter_.filter_order_, low, high, parameter_.filter_type_);

    // 1.2确定滤波函数
    std::shared_ptr<numerical_algorithm::BasicFiltering> filter_function =
        nullptr;
    switch (parameter_.filter_function_)
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
    numerical_algorithm::Interp interp_function(parameter_.interp_type_);

    // 2.滤波积分插值计算层间位移角
    double dt = input_acceleration_.get_time_step();
    result_->measurement_avd_.frequency = input_acceleration_.get_frequency();
    std::vector<std::vector<double>>
        &filtered_acc = result_->measurement_avd_.acceleration.data(),
        &filtered_vel = result_->measurement_avd_.velocity.data(),
        &filtered_disp = result_->measurement_avd_.displacement.data();
    // 2.1 加速度滤波
    filtered_acc = filter_function->Filtering(input_acceleration_.get_data());
    //  2.2 加速度积分到速度
    auto velocity = numerical_algorithm::Cumtrapz(filtered_acc, dt);
    // 2.3 速度滤波
    filtered_vel = filter_function->Filtering(velocity);
    // 2.4 速度积分到位移
    auto displacement = numerical_algorithm::Cumtrapz(filtered_vel, dt);
    // 2.5 位移滤波
    filtered_disp = filter_function->Filtering(displacement);
    // 2.6 位移插值
    result_->avd_.displacement.data() =
        interp_function.Interpolation(building_ptr_->get_measuren_height(),
                                      filtered_disp,
                                      building_ptr_->get_floor_height());
    // 2.7 计算层间位移
    auto interstory_displacement =
        result_->get_displacement().interstory_displacement();
    auto interstory_height = building_ptr_->get_inter_height();

    // 2.8 计算层间位移角
    for (std::size_t i = 0; i < interstory_displacement.data().size(); ++i)
    {
        result_->inter_story_drift_.data().push_back(
            numerical_algorithm::VectorOperation(
                interstory_displacement.data()[i], interstory_height[i], '/'));
    }
    result_->avd_.frequency = input_acceleration_.get_frequency();

    // 传递部分信息用于计算非测点的数据
    result_->interp_type_ = parameter_.interp_type_;
    result_->building_ = building_ptr_;

    // 3.计算完成
    is_calculated_ = true;
}
} // namespace edp_calculation