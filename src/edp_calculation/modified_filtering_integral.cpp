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
#include <vector>

// third-party library headers
#include <eigen3/Eigen/Dense>

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
    std::shared_ptr<numerical_algorithm::Interp> interp_function = nullptr;
    switch (method_.interp_type_)
    {
        case numerical_algorithm::InterpType::Linear:
            interp_function->set_interp_type(
                numerical_algorithm::InterpType::Linear);
            break;
        case numerical_algorithm::InterpType::CubicSpline:
            interp_function->set_interp_type(
                numerical_algorithm::InterpType::CubicSpline);
            break;
        case numerical_algorithm::InterpType::Akima:
            interp_function->set_interp_type(
                numerical_algorithm::InterpType::Akima);
            break;
        case numerical_algorithm::InterpType::Steffen:
            interp_function->set_interp_type(
                numerical_algorithm::InterpType::Steffen);
            break;
        case numerical_algorithm::InterpType::Polynomial:
            interp_function->set_interp_type(
                numerical_algorithm::InterpType::Polynomial);
            break;
        default:
            interp_function->set_interp_type(
                numerical_algorithm::InterpType::Linear);
            break;
    }

    // 2.滤波积分插值计算层间位移角

    // 2.2 逐列滤波积分
    Eigen::MatrixXd filtered_displacement(
        input_acceleration_ptr_->get_data().rows(),
        input_acceleration_ptr_->get_data().cols());
    for (std::size_t i = 0; i < input_acceleration_ptr_->get_data().cols(); ++i)
    {
        filtered_displacement.col(i) = CalculateSingle(i);
    }

    // 2.6 位移插值
    result_ptr_->displacement_ptr_ =
        std::make_shared<data_structure::Displacement>(
            Eigen::MatrixXd(), input_acceleration_ptr_->get_frequency());
    interp_function->Interpolation(building_ptr_->get_measuren_height(),
                                   filtered_displacement,
                                   building_ptr_->get_floor_height(),
                                   result_ptr_->displacement_ptr_->data());
    // 2.7 计算层间位移
    auto interstory_displacement =
        result_ptr_->displacement_ptr_->interstory_displacement();
    result_ptr_->story_drift_ptr_ =
        std::make_shared<data_structure::StoryDrift>(
            Eigen::MatrixXd::Zero(interstory_displacement.data().rows(),
                                  interstory_displacement.data().cols()));
    auto interstory_height = Eigen::Map<Eigen::RowVectorXd>(
        building_ptr_->get_inter_height().data(),
        building_ptr_->get_inter_height().size());
    for (std::size_t i = 0; i < interstory_displacement.data().rows(); ++i)
    {
        result_ptr_->story_drift_ptr_->data().row(i) =
            interstory_displacement.data().row(i).array()
            / interstory_height.array();
    }
}

// 滤波积分插值法计算单列加速度
Eigen::MatrixXd
ModifiedFilteringIntegral::CalculateSingle(const std::size_t &col)
{
    // 1.初步到速度和位移
    double dt = input_acceleration_ptr_->get_time_step();

    auto velocity_0 = Eigen::MatrixXd();
    numerical_algorithm::Cumtrapz(
        input_acceleration_ptr_->get_data().col(col), velocity_0, dt);
    auto displacement_0 = Eigen::MatrixXd();
    numerical_algorithm::Cumtrapz(velocity_0, displacement_0, dt);

    // 2.滤波积分，得到不同低频限值下的结果
    int max_k = 100;
    // 储存每次滤波积分的临时结果
    Eigen::MatrixXd integral_velocity, integral_displacement,
        filtered_acceleration_0, filtered_velocity_0, filtered_displacement_0;
    auto filter_generator = numerical_algorithm::ButterworthFilterDesign();
    auto filter_function = numerical_algorithm::FiltFilt();
    double low, high = high_frequency_;
    for (int i = 0; i < max_k; ++i)
    {
        // 2.1 设置滤波参数
        low = 1.0 * (i + 1) / max_k;
        filter_generator.set_frequency(low, high);
        filter_function.set_coefficients(filter_generator);

        // 2.2 滤波积分
        filter_function.Filtering(input_acceleration_ptr_->get_data().col(col),
                                  filtered_acceleration_0);
        numerical_algorithm::Cumtrapz(
            filtered_acceleration_0, integral_velocity, dt);
        filter_function.Filtering(integral_velocity, filtered_velocity_0);
        numerical_algorithm::Cumtrapz(
            filtered_velocity_0, integral_displacement, dt);
        filter_function.Filtering(integral_displacement,
                                  filtered_displacement_0);
    }

    // 3.根据功率比选择最佳结果
    // 3.1 计算功率比
    std::vector<double> power_ratio(max_k, 0.0);
    power_ratio[0] = filtered_acceleration_0.col(1).array().pow(2).sum()
                     / displacement_0.array().pow(2).sum();
    for (int i = 1; i < max_k; ++i)
    {
        power_ratio[i] =
            filtered_acceleration_0.col(i).array().pow(2).sum()
            / filtered_displacement_0.col(i - 1).array().pow(2).sum();
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
    return filtered_displacement_0.col(nth_fre);
}
} // namespace edp_calculation