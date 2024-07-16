/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/edp_calculation/modified_filtering_integral.cpp
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
// Description:
// 工程需求参量计算方法：改进的滤波积分插值法，基于最大功率比的滤波方法
// 仅计算步骤和简单滤波积分方法有差别，成员变量和基本方法相同
// 算法提供者：2021级硕士陈鹏杰（Chen Pengjie）

// associated header
#include "modified_filtering_integral.h"

// stdc++ headers
#include <memory>
#include <numeric>
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
void ModifiedFilteringIntegral::CalculateEdp()
{
    // 1.确定计算参数
    // 1.1确定滤波生成器
    auto filter_generator = numerical_algorithm::ButterworthFilterDesign(
        filter_order_, 0, 0, method_.filter_type_);

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
    // 2.1 逐列滤波积分得到测点位移
    std::vector<std::vector<double>> filtered_displacement(
        input_acceleration_ptr_->get_data().size(),
        std::vector<double>(input_acceleration_ptr_->get_data().size(), 0.0));
    for (std::size_t i = 0; i < input_acceleration_ptr_->get_data().size(); ++i)
    {
        filtered_displacement[i] = CalculateSingle(i);
    }

    // 2.2 位移插值
    result_ptr_->displacement_ptr_ =
        std::make_shared<data_structure::Displacement>(
            std::vector<std::vector<double>>(),
            input_acceleration_ptr_->get_frequency());
    result_ptr_->displacement_ptr_->data() =
        interp_function.Interpolation(building_ptr_->get_measuren_height(),
                                      filtered_displacement,
                                      building_ptr_->get_floor_height());
    // 2.3 计算层间位移
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

// 滤波积分插值法计算单列加速度
std::vector<double>
ModifiedFilteringIntegral::CalculateSingle(const std::size_t &col)
{
    // 1.初步到速度和位移
    double dt = input_acceleration_ptr_->get_time_step();

    auto velocity_0 = numerical_algorithm::Cumtrapz(
        input_acceleration_ptr_->get_data()[col], dt);
    auto displacement_0 = numerical_algorithm::Cumtrapz(velocity_0, dt);

    // 2.滤波积分，得到不同低频限值下的结果
    int max_k = 100;
    // 2.1储存每次滤波积分的临时结果
    std::vector<std::vector<double>> integral_velocity(max_k),
        integral_displacement(max_k), filtered_acceleration_0(max_k),
        filtered_velocity_0(max_k), filtered_displacement_0(max_k);
    // 2.2设置滤波积分方法
    auto filter_generator = numerical_algorithm::ButterworthFilterDesign(2);
    auto filter_function = numerical_algorithm::FiltFilt();
    double low, high = 20;
    for (int i = 0; i < max_k; ++i)
    {
        // 2.3 设置滤波参数
        low = 1.0 * (i + 1) / max_k;
        filter_generator.set_frequency(low, high);
        filter_generator.DesignFilter();
        filter_function.set_coefficients(filter_generator);

        // 2.4 滤波积分
        filtered_acceleration_0[i] =
            filter_function.Filtering(input_acceleration_ptr_->get_data()[col]);
        integral_velocity[i] =
            numerical_algorithm::Cumtrapz(filtered_acceleration_0[i], dt);
        filtered_velocity_0[i] =
            filter_function.Filtering(integral_velocity[i]);
        integral_displacement[i] =
            numerical_algorithm::Cumtrapz(filtered_velocity_0[i], dt);
        filtered_displacement_0[i] =
            filter_function.Filtering(integral_displacement[i]);
    }

    // 3.根据功率比选择最佳结果
    // 3.1 计算功率比
    std::vector<double> power_ratio(max_k, 0.0);
    power_ratio[0] =
        std::accumulate(filtered_acceleration_0[0].begin(),
                        filtered_acceleration_0[0].end(),
                        0.0,
                        [](double x, double y) { return x + y * y; })
        / std::accumulate(displacement_0.begin(),
                          displacement_0.end(),
                          0.0,
                          [](double x, double y) { return x + y * y; });
    for (int i = 1; i < max_k; ++i)
    {
        power_ratio[i] =
            std::accumulate(filtered_acceleration_0[i].begin(),
                            filtered_acceleration_0[i].end(),
                            0.0,
                            [](double x, double y) { return x + y * y; })
            / std::accumulate(filtered_acceleration_0[i - 1].begin(),
                              filtered_acceleration_0[i - 1].end(),
                              0.0,
                              [](double x, double y) { return x + y * y; });
    }

    // 3.2 选择功率比最大的结果
    int nth_fre, nth_fre_defalt = 30;
    auto peaks = numerical_algorithm::FindPeaks(power_ratio);
    if (peaks.size())
    {
        auto nfre_peak_index =
            std::find_if(power_ratio.begin(),
                         power_ratio.end(),
                         [](const double &x) { return x > 0.9; });
        if (nfre_peak_index != power_ratio.end())
        {
            nth_fre = nth_fre_defalt;
        }
        else
        {
            nth_fre = peaks[0];
        }
    }
    else
    {
        nth_fre = nth_fre_defalt;
    }
    return filtered_displacement_0[nth_fre];
}
} // namespace edp_calculation