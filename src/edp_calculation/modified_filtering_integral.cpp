/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\edp_calculation\modified_filtering_integral.cpp
** -----
** File Created: Monday, 15th July 2024 15:04:27
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Monday, 4th November 2024 00:35:06
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

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
#include <data_structure/displacement.h>
#include "numerical_algorithm/butterworth_filter_design.h"
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
    // 以下内容是为了从多种滤波函数中选取一种进行计算的，原算法只是使用了FiltFilt滤波
    // 是否需要增加额外内容待定，如果加，可在CalculateSingle()中增加一个滤波基类参数
    // // 1.1确定滤波生成器
    // auto filter_generator = numerical_algorithm::ButterworthFilterDesign(
    //     method_.filter_order_, 0, 0, method_.filter_type_);
    // auto filter_coeff = filter_generator.get_filter_coefficients();
    // // 1.2确定滤波函数
    // std::shared_ptr<numerical_algorithm::BasicFiltering> filter_function =
    //     nullptr;
    // switch (method_.filter_function_)
    // {
    //     case numerical_algorithm::FilterFunction::filtfilt:
    //         filter_function =
    //         std::make_shared<numerical_algorithm::FiltFilt>(
    //             filter_coeff.first, filter_coeff.second);
    //         break;
    //     case numerical_algorithm::FilterFunction::filter:
    //         filter_function = std::make_shared<numerical_algorithm::Filter>(
    //             filter_coeff.first, filter_coeff.second);
    //         break;
    //     default:
    //         filter_function =
    //         std::make_shared<numerical_algorithm::FiltFilt>(
    //             filter_coeff.first, filter_coeff.second);
    //         break;
    // }

    // 1.3确定插值方法
    numerical_algorithm::Interp interp_function(parameter_.interp_type_);

    // 2.滤波积分插值计算层间位移角
    // 2.1 逐列滤波积分得到测点位移
    // 初始化内存分配
    const std::size_t sz = input_acceleration_.get_data().size();
    std::vector<std::vector<double>>
        &filtered_disp = result_->measurement_avd_.displacement.data(),
        &filtered_vel = result_->measurement_avd_.velocity.data(),
        &filtered_acc = result_->measurement_avd_.acceleration.data();
    filtered_disp.resize(sz);
    filtered_vel.resize(sz);
    filtered_acc.resize(sz);
    // 传递部分信息用于计算非测点的数据
    result_->interp_type_ = parameter_.interp_type_;
    result_->building_ = building_ptr_;
    result_->measurement_avd_.frequency = input_acceleration_.get_frequency();
    // 依次计算各测点的数据
    for (std::size_t i = 0; i < input_acceleration_.get_data().size(); ++i)
    {
        CalculateSingle(i);
    }

    // 2.2 位移插值
    result_->avd_.frequency = input_acceleration_.get_frequency();
    result_->avd_.displacement.data() =
        interp_function.Interpolation(building_ptr_->get_measuren_height(),
                                      filtered_disp,
                                      building_ptr_->get_floor_height());
    // 2.3 计算层间位移
    auto interstory_displacement =
        result_->get_displacement().interstory_displacement();
    auto interstory_height = building_ptr_->get_inter_height();
    // 2.4 计算层间位移角
    for (std::size_t i = 0; i < interstory_displacement.data().size(); ++i)
    {
        result_->inter_story_drift_.data().push_back(
            numerical_algorithm::VectorOperation(
                interstory_displacement.data()[i], interstory_height[i], '/'));
    }

    // 3.计算完成
    is_calculated_ = true;
}

// 滤波积分插值法计算单列加速度
void ModifiedFilteringIntegral::CalculateSingle(const std::size_t &col)
{
    // 1.初步到速度和位移
    double dt = input_acceleration_.get_time_step();

    auto velocity_0 =
        numerical_algorithm::Cumtrapz(input_acceleration_.get_data()[col], dt);
    auto displacement_0 = numerical_algorithm::Cumtrapz(velocity_0, dt);

    // 2.滤波积分，得到不同低频限值下的结果
    int max_k = 100;
    // 2.1储存每次滤波积分的临时结果
    std::vector<std::vector<double>> integral_velocity(max_k),
        integral_displacement(max_k), filtered_acceleration(max_k),
        filtered_velocity(max_k), filtered_displacement(max_k);
    // 2.2设置滤波积分方法
    auto filter_generator =
        numerical_algorithm::ButterworthFilterDesign(parameter_.filter_order_);
    auto filter_function = numerical_algorithm::FiltFilt();
    double low, high = 20.0 / input_acceleration_.get_frequency() * 2;
    double low_scale = high / 20;
    for (int i = 0; i < max_k; ++i)
    {
        // 2.3 设置滤波参数
        low = 1.0 * (i + 1) / max_k * low_scale;
        filter_generator.set_frequency(low, high);
        filter_generator.DesignFilter();
        filter_function.set_coefficients(filter_generator);

        // 2.4 滤波积分
        filtered_acceleration[i] =
            filter_function.Filtering(input_acceleration_.get_data()[col]);
        integral_velocity[i] =
            numerical_algorithm::Cumtrapz(filtered_acceleration[i], dt);
        filtered_velocity[i] = filter_function.Filtering(integral_velocity[i]);
        integral_displacement[i] =
            numerical_algorithm::Cumtrapz(filtered_velocity[i], dt);
        filtered_displacement[i] =
            filter_function.Filtering(integral_displacement[i]);
    }

    // 3.根据功率比选择最佳结果
    // 3.1 计算功率比
    std::vector<double> power_ratio(max_k, 0.0);
    power_ratio[0] =
        std::accumulate(filtered_displacement[0].begin(),
                        filtered_displacement[0].end(),
                        0.0,
                        [](double x, double y) { return x + y * y; })
        / std::accumulate(displacement_0.begin(),
                          displacement_0.end(),
                          0.0,
                          [](double x, double y) { return x + y * y; });
    for (std::size_t i = 1; i < max_k; ++i)
    {
        power_ratio[i] =
            std::accumulate(filtered_displacement[i].begin(),
                            filtered_displacement[i].end(),
                            0.0,
                            [](double x, double y) { return x + y * y; })
            / std::accumulate(filtered_displacement[i - 1].begin(),
                              filtered_displacement[i - 1].end(),
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
        if (nfre_peak_index == power_ratio.end())
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
    result_->measurement_avd_.displacement.data()[col] =
        filtered_displacement[nth_fre];
    result_->measurement_avd_.velocity.data()[col] = filtered_velocity[nth_fre];
    result_->measurement_avd_.acceleration.data()[col] =
        filtered_acceleration[nth_fre];
}
} // namespace edp_calculation