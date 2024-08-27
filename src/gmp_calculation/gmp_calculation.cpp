/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\gmp_calculation\gmp_calculation.cpp
** -----
** File Created: Tuesday, 23rd July 2024 22:50:47
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Saturday, 10th August 2024 22:27:14
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 计算地震参数类的实现。

// associated header
#include "gmp_calculation.h"

// stdc++ headers
#include <cmath>
#include <fstream>
#include <vector>

// third-party library headers
#include "nlohmann/json.hpp"

// project headers
#include "numerical_algorithm/vector_calculation.h"

namespace gmp_calculation
{
// 从std::vector构造
inline GmpCalculation::GmpCalculation(const std::vector<double> &acceleration,
                                      double frequency,
                                      double damping_ratio)
    : acceleration_ptr_(std::make_shared<std::vector<double>>(acceleration))
{
    parameter_.frequency_ = frequency;
    parameter_.time_step_ = 1.0 / frequency;
    parameter_.damping_ratio_ = damping_ratio;
}


// 从std::vector指针构造
inline GmpCalculation::GmpCalculation(
    const std::shared_ptr<std::vector<double>> &acceleration_ptr,
    double frequency,
    double damping_ratio)
    : acceleration_ptr_(acceleration_ptr)
{
    parameter_.frequency_ = frequency;
    parameter_.time_step_ = 1.0 / frequency;
    parameter_.damping_ratio_ = damping_ratio;
}

// 从配置文件中读取参数构造
GmpCalculation::GmpCalculation(const std::vector<double> &acceleration)
    : acceleration_ptr_(std::make_shared<std::vector<double>>(acceleration))
{
    LoadConfig();
}

// NewmakeBeta方法计算响应
ResponseSpectrumTiResult GmpCalculation::NewmarkBeta(const double &Ti)
{
    double dt = parameter_.time_step_;
    double beta = 0.25, gamma = 0.5;
    double omega = 2 * M_PI / Ti;
    double k = omega * omega, c = 2 * parameter_.damping_ratio_ * omega;
    double p1 = 1 / (beta * dt * dt);
    double p2 = gamma / (beta * dt);
    double p3 = 1 / (beta * dt);
    double p4 = gamma / beta - 1;
    double p5 = 1 / (2 * beta) - 1;
    double p6 = dt * (gamma / 2 / beta - 1);
    double a1 = p1 + p2 * c, a2 = p3 + p4 * c, a3 = p5 + p6 * c;
    double k_b = k + a1;
    // 初始化
    std::vector<double> u(acceleration_ptr_->size()),
        v(acceleration_ptr_->size()), a(acceleration_ptr_->size()),
        a_(acceleration_ptr_->size());
    double p = -acceleration_ptr_->at(0);
    u[0] = 0;
    v[0] = 0;
    a[0] = p;
    // 迭代
    for (size_t i = 1; i != acceleration_ptr_->size(); i++)
    {
        p = -acceleration_ptr_->at(i) + a1 * u[i - 1] + a2 * v[i - 1]
            + a3 * a[i - 1];
        u[i] = p / k_b;
        v[i] = p2 * (u[i] - u[i - 1]) - p4 * v[i - 1] - p6 * a[i - 1];
        a[i] = p1 * (u[i] - u[i - 1]) - p3 * v[i - 1] - p5 * a[i - 1];
        a_[i] = a[i] + acceleration_ptr_->at(i);
    }
    // 计算结果
    response_spectrum_ti_.SaTi = numerical_algorithm::FindMaxAbs(a_);
    response_spectrum_ti_.SvTi = numerical_algorithm::FindMaxAbs(v);
    response_spectrum_ti_.SdTi = numerical_algorithm::FindMaxAbs(u);
    return response_spectrum_ti_;
}

// 从配置文件中读取参数
void GmpCalculation::LoadConfig(const std::string &config_file)
{
    // 导入新配置需要清除已有结果
    clear_result();

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

    // 读取参数
    parameter_.frequency_ = config["AccConfig"]["frequency"];
    parameter_.time_step_ = 1.0 / parameter_.frequency_;
    parameter_.damping_ratio_ =
        config["ResponseSpectrumConfig"]["damping_ratio"];
    parameter_.response_spectrum_dt_ =
        config["ResponseSpectrumConfig"]["period_step"];
    parameter_.response_spectrum_max_period_ =
        config["ResponseSpectrumConfig"]["max_period"];
    parameter_.fourier_spectrum_max_frequency_ =
        config["FourierConfig"]["max_frequency"];
}

} // namespace gmp_calculation