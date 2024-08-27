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
** Last Modified: Wednesday, 21st August 2024 15:42:16
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 工程需求参量计算方法：滤波积分插值法的实现

// associated header
#include "filtering_integral.h"

// stdc++ headers
#include <fstream>
#include <memory>
#include <vector>

// third-party library headers
#include "nlohmann/json.hpp"

// project headers
#include "data_structure/displacement.h"

#include "numerical_algorithm/basic_filtering.h"
#include "numerical_algorithm/butterworth_filter_design.h"
#include "numerical_algorithm/filter.h"
#include "numerical_algorithm/filtfilt.h"
#include "numerical_algorithm/integral.h"
#include "numerical_algorithm/interp.h"
#include "numerical_algorithm/vector_calculation.h"


namespace edp_calculation
{

// 读取配置文件
void FilteringIntegral::LoadConfig(const std::string &config_file)
{
    // JSON配置文件
    nlohmann::json config;
    std::ifstream ifs(config_file);
    if (ifs.is_open())
    {
        ifs >> config;
        ifs.close();
    }
    else
    {
        throw std::runtime_error("Cannot open the configuration file.");
    }

    method_.filter_order_ = config["FilterConfig"]["filter_order"];
    method_.low_frequency_ = config["FilterConfig"]["low_frequency"];
    method_.high_frequency_ = config["FilterConfig"]["high_frequency"];
    method_.filter_type_ = config["FilterConfig"]["filter_type"];
    method_.filter_function_ = config["FilterConfig"]["filter_function"];
    method_.filter_generator_ = config["FilterConfig"]["filter_generator"];
    method_.interp_type_ = config["InterpConfig"]["interp_type"];
}

// 滤波积分插值法计算的入口
void FilteringIntegral::CalculateEdp()
{
    // 1.确定计算参数
    // 1.1确定滤波生成器
    double low =
               method_.low_frequency_ / input_acceleration_.get_frequency() * 2,
           high = method_.high_frequency_ / method_.low_frequency_ * low;
    auto filter_generator = numerical_algorithm::ButterworthFilterDesign(
        method_.filter_order_, low, high, method_.filter_type_);

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
    double dt = input_acceleration_.get_time_step();
    // 2.1 加速度滤波
    auto filtered_acceleration =
        filter_function->Filtering(input_acceleration_.get_data());
    //  2.2 加速度积分到速度
    auto velocity = numerical_algorithm::Cumtrapz(filtered_acceleration, dt);
    // 2.3 速度滤波
    auto interp_velocity = filter_function->Filtering(velocity);
    // 2.4 速度积分到位移
    auto displacement = numerical_algorithm::Cumtrapz(interp_velocity, dt);
    // 2.5 位移滤波
    auto filtered_displacement = filter_function->Filtering(displacement);
    // 2.6 位移插值
    result_.displacement_.set_frequency(input_acceleration_.get_frequency());
    result_.displacement_.data() =
        interp_function.Interpolation(building_.get_measuren_height(),
                                      filtered_displacement,
                                      building_.get_floor_height());
    // 2.7 计算层间位移
    auto interstory_displacement =
        result_.displacement_.interstory_displacement();
    auto interstory_height = building_.get_inter_height();
    // 2.8 计算层间位移角
    for (std::size_t i = 0; i < interstory_displacement.data().size(); ++i)
    {
        result_.inter_story_drift_.data().push_back(
            numerical_algorithm::VectorOperation(
                interstory_displacement.data()[i], interstory_height[i], '/'));
    }
}
} // namespace edp_calculation