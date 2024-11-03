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
#include "numerical_algorithm/filter.h"
#include "numerical_algorithm/vector_calculation.h"

namespace gmp_calculation
{
// 从std::vector构造
GmpCalculation::GmpCalculation(const std::vector<double> &acceleration,
                               double frequency,
                               double damping_ratio)
    : ori_acceleration_(acceleration)
{
    // 加载和更新配置
    LoadConfig();
    parameter_.frequency_ = frequency;
    parameter_.time_step_ = 1.0 / frequency;
    parameter_.damping_ratio_ = damping_ratio;

    // 是否初滤波
    if (parameter_.filter_)
    {
        // 始化滤波器
        init_filter();
        // 加速度信号滤波
        acceleration_ = filter_function_.Filtering(acceleration);
    }
    else
    {
        acceleration_ = acceleration;
    }
}


// 从std::vector指针构造
GmpCalculation::GmpCalculation(
    const std::shared_ptr<std::vector<double>> &acceleration_ptr,
    double frequency,
    double damping_ratio)
    : ori_acceleration_(*acceleration_ptr)
{
    // 加载和更新配置
    LoadConfig();
    parameter_.frequency_ = frequency;
    parameter_.time_step_ = 1.0 / frequency;
    parameter_.damping_ratio_ = damping_ratio;

    // 是否初滤波
    if (parameter_.filter_)
    {
        // 始化滤波器
        init_filter();
        // 加速度信号滤波
        acceleration_ = filter_function_.Filtering(*acceleration_ptr);
    }
    else
    {
        acceleration_ = *acceleration_ptr;
    }
}

// 从配置文件中读取参数构造
GmpCalculation::GmpCalculation(const std::vector<double> &acceleration)
    : ori_acceleration_(acceleration)
{
    // 加载配置
    LoadConfig();

    // 是否初滤波
    if (parameter_.filter_)
    {
        // 始化滤波器
        init_filter();
        // 加速度信号滤波
        acceleration_ = filter_function_.Filtering(acceleration);
    }
    else
    {
        acceleration_ = acceleration;
    }
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
    std::vector<double> u(acceleration_.size()), v(acceleration_.size()),
        a(acceleration_.size()), a_(acceleration_.size());
    double p = -acceleration_.at(0);
    u[0] = 0;
    v[0] = 0;
    a[0] = p;
    // 迭代
    for (size_t i = 1; i != acceleration_.size(); i++)
    {
        p = -acceleration_.at(i) + a1 * u[i - 1] + a2 * v[i - 1]
            + a3 * a[i - 1];
        u[i] = p / k_b;
        v[i] = p2 * (u[i] - u[i - 1]) - p4 * v[i - 1] - p6 * a[i - 1];
        a[i] = p1 * (u[i] - u[i - 1]) - p3 * v[i - 1] - p5 * a[i - 1];
        a_[i] = a[i] + acceleration_.at(i);
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
    // 计算与绘图参数
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

    // 滤波相关参数
    parameter_.filter_ = config["GMPFilterConfig"]["filter_flag"];
    parameter_.filter_order_ = config["GMPFilterConfig"]["filter_order"];
    parameter_.low_frequency_ = config["GMPFilterConfig"]["low_frequency"];
    parameter_.high_frequency_ = config["GMPFilterConfig"]["high_frequency"];
    parameter_.filter_type_ = config["GMPFilterConfig"]["filter_type"];
    parameter_.filter_function_ = config["GMPFilterConfig"]["filter_function"];
    parameter_.filter_generator_ =
        config["GMPFilterConfig"]["filter_generator"];
}

// 更新参数
void GmpCalculation::Update()
{
    // 是否初滤波
    if (parameter_.filter_)
    {
        // 始化滤波器
        init_filter();
        // 加速度信号滤波
        acceleration_ = filter_function_.Filtering(ori_acceleration_);
    }
    else
    {
        acceleration_ = ori_acceleration_;
    }
    clear_result();
}

/** 读取和设置参数 **/

// 设置加速度数据
void GmpCalculation::set_acceleration(const std::vector<double> &acceleration)
{
    ori_acceleration_ = acceleration;
    // 是否初滤波
    if (parameter_.filter_)
    {
        // 加速度信号滤波
        acceleration_ = filter_function_.Filtering(acceleration);
    }
    else
    {
        acceleration_ = acceleration;
    }
    clear_result();
}

// 设置频率
void GmpCalculation::set_frequency(double frequency)
{
    parameter_.frequency_ = frequency;
    parameter_.time_step_ = 1.0 / frequency;
    clear_result();
}

// 设置阻尼比
void GmpCalculation::set_damping_ratio(double damping_ratio)
{
    parameter_.damping_ratio_ = damping_ratio;
    clear_result();
}

// 获取加速度数据
std::vector<double> &GmpCalculation::get_acceleration()
{
    return acceleration_;
}

// 获取频率
double GmpCalculation::get_frequency() const { return parameter_.frequency_; }

// 获取阻尼比
double GmpCalculation::get_damping_ratio() const
{
    return parameter_.damping_ratio_;
}

// 获取速度
std::vector<double> GmpCalculation::get_velocity()
{
    if (velocity_.empty())
    {
        calculate_velocity();
    }
    return velocity_;
}

// 获取位移
std::vector<double> GmpCalculation::get_displacement()
{
    if (displacement_.empty())
    {
        calculate_displacement();
    }
    return displacement_;
}

/** 峰值信息 */

// 获取峰值加速度
double GmpCalculation::PeakAcceleration() const
{
    return numerical_algorithm::FindMaxAbs(acceleration_);
}

// 获取峰值速度
double GmpCalculation::PeakVelocity()
{
    if (velocity_.empty())
    {
        calculate_velocity();
    }
    return numerical_algorithm::FindMaxAbs(velocity_);
}

// 获取峰值位移
double GmpCalculation::PeakDisplacement()
{
    if (displacement_.empty())
    {
        calculate_displacement();
    }
    return numerical_algorithm::FindMaxAbs(displacement_);
}

// 获取加速度均方根
double GmpCalculation::RmsAcceleration()
{
    double sum = std::accumulate(acceleration_.begin(),
                                 acceleration_.end(),
                                 0.0,
                                 [](double a, double b) { return a + b * b; });
    return std::sqrt(sum / acceleration_.size());
}

// 获取速度均方根
double GmpCalculation::RmsVelocity()
{
    if (velocity_.empty())
    {
        calculate_velocity();
    }
    double sum = std::accumulate(velocity_.begin(),
                                 velocity_.end(),
                                 0.0,
                                 [](double a, double b) { return a + b * b; });
    return std::sqrt(sum / velocity_.size());
}

// 获取位移均方根
double GmpCalculation::RmsDisplacement()
{
    if (displacement_.empty())
    {
        calculate_displacement();
    }
    double sum = std::accumulate(displacement_.begin(),
                                 displacement_.end(),
                                 0.0,
                                 [](double a, double b) { return a + b * b; });
    return std::sqrt(sum / displacement_.size());
}

/** 持时信息 **/

// 获取持时
double GmpCalculation::Duration()
{
    double peak_acceleration = PeakAcceleration();
    auto it1 = std::find_if(acceleration_.begin(),
                            acceleration_.end(),
                            [peak_acceleration](double a) {
                                return std::abs(a) > 0.05 * peak_acceleration;
                            });
    auto it2 = std::find_if(acceleration_.rbegin(),
                            acceleration_.rend(),
                            [peak_acceleration](double a) {
                                return std::abs(a) > 0.05 * peak_acceleration;
                            });
    return std::distance(it1, it2.base()) * parameter_.time_step_;
}

/** 地震反应谱 **/

// 获取反应谱计算结果
ResponseSpectrumResult GmpCalculation::get_response_spectrum()
{
    if (!response_spectrum_flag_)
    {
        ResponseSpectrum();
    }
    return response_spectrum_;
}

// 获取Ti周期下反应谱谱值
ResponseSpectrumTiResult GmpCalculation::ResponseSpectrumTi(const double &Ti)
{
    return NewmarkBeta(Ti);
}

// 获取Ti周期下加速度反应谱谱值
double GmpCalculation::AccelerationSpectrumTi(const double &Ti)
{
    return NewmarkBeta(Ti).SaTi;
}

// 获取Ti周期下速度反应谱谱值
double GmpCalculation::VelocitySpectrumTi(const double &Ti)
{
    return NewmarkBeta(Ti).SvTi;
}

// 获取Ti周期下位移反应谱谱值
double GmpCalculation::DisplacementSpectrumTi(const double &Ti)
{
    return NewmarkBeta(Ti).SdTi;
}

// 获取反应谱
ResponseSpectrumResult GmpCalculation::ResponseSpectrum()
{
    clear_result();
    int period_length = parameter_.response_spectrum_max_period_
                        / parameter_.response_spectrum_dt_;
    for (int Ti = 1; Ti <= period_length; ++Ti)
    {
        NewmarkBeta(Ti * parameter_.response_spectrum_dt_);
        response_spectrum_.Sa.push_back(response_spectrum_ti_.SaTi);
        response_spectrum_.Sv.push_back(response_spectrum_ti_.SvTi);
        response_spectrum_.Sd.push_back(response_spectrum_ti_.SdTi);
    }
    response_spectrum_flag_ = true;
    return response_spectrum_;
}

// 获取加速度反应谱
std::vector<double> GmpCalculation::AccelerationSpectrum()
{
    if (!response_spectrum_flag_)
    {
        ResponseSpectrum();
    }
    return response_spectrum_.Sa;
}

// 获取速度反应谱
std::vector<double> GmpCalculation::VelocitySpectrum()
{
    if (!response_spectrum_flag_)
    {
        ResponseSpectrum();
    }
    return response_spectrum_.Sv;
}

// 获取位移反应谱
std::vector<double> GmpCalculation::DisplacementSpectrum()
{
    if (!response_spectrum_flag_)
    {
        ResponseSpectrum();
    }
    return response_spectrum_.Sd;
}

// 获取拟反应谱计算结果
ResponseSpectrumResult GmpCalculation::get_pseudo_response_spectrum()
{
    if (!pesudo_response_spectrum_flag_)
    {
        PseudoResponseSpectrum();
    }
    return pesudo_response_spectrum_;
}

// 获取Ti周期下拟反应谱谱值
ResponseSpectrumTiResult
GmpCalculation::PseudoResponseSpectrumTi(const double &Ti)
{
    ResponseSpectrumTi(Ti);

    const double omega = 2 * M_PI / Ti;
    pesudo_response_spectrum_ti_.SaTi =
        response_spectrum_ti_.SaTi * omega * omega;
    pesudo_response_spectrum_ti_.SvTi = response_spectrum_ti_.SvTi * omega;
    pesudo_response_spectrum_ti_.SdTi = response_spectrum_ti_.SdTi;
    return pesudo_response_spectrum_ti_;
}

// 获取Ti周期下拟加速度反应谱谱值
double GmpCalculation::PseudoAccelerationSpectrumTi(const double &Ti)
{
    const double omega = 2 * M_PI / Ti;
    return ResponseSpectrumTi(Ti).SaTi * omega * omega;
}

// 获取Ti周期下拟速度反应谱谱值
double GmpCalculation::PseudoVelocitySpectrumTi(const double &Ti)
{
    const double omega = 2 * M_PI / Ti;
    return ResponseSpectrumTi(Ti).SvTi * omega;
}

// 获取拟反应谱
ResponseSpectrumResult GmpCalculation::PseudoResponseSpectrum()
{
    double period_length = parameter_.response_spectrum_max_period_
                           / parameter_.response_spectrum_dt_;
    double omega;
    if (!response_spectrum_flag_)
    {
        ResponseSpectrum();
    }
    for (int Ti = 1; Ti <= period_length; ++Ti)
    {
        omega = 2 * M_PI / Ti * parameter_.response_spectrum_dt_;
        pesudo_response_spectrum_.Sa.push_back(response_spectrum_.Sd[Ti - 1]
                                               * omega * omega);
        pesudo_response_spectrum_.Sv.push_back(response_spectrum_.Sd[Ti - 1]
                                               * omega);
        pesudo_response_spectrum_.Sd.push_back(response_spectrum_.Sd[Ti - 1]);
    }
    pesudo_response_spectrum_flag_ = true;
    return pesudo_response_spectrum_;
}

// 获取拟加速度反应谱
std::vector<double> GmpCalculation::PseudoAccelerationSpectrum()
{
    if (!pesudo_response_spectrum_flag_)
    {
        PseudoResponseSpectrum();
    }
    return pesudo_response_spectrum_.Sa;
}

// 获取拟速度反应谱
std::vector<double> GmpCalculation::PseudoVelocitySpectrum()
{
    if (!pesudo_response_spectrum_flag_)
    {
        PseudoResponseSpectrum();
    }
    return pesudo_response_spectrum_.Sv;
}

/** 频域信息 **/

// 获取Fourier幅值谱
std::vector<double> GmpCalculation::FourierAmplitudeSpectrum()
{
    if (fourier_transform_result_.empty())
    {
        fourier_transform();
    }
    std::size_t max_index =
        static_cast<std::size_t>(parameter_.fourier_spectrum_max_frequency_
                                 / parameter_.fourier_spectrum_df_);
    std::vector<double> amplitude_spectrum(max_index);
    for (size_t i = 0; i != max_index; i++)
    {
        amplitude_spectrum[i] = std::abs(fourier_transform_result_[i]);
    }
    if (max_index == 0)
        return amplitude_spectrum;
    else
    {
        return std::vector<double>(amplitude_spectrum.begin(),
                                   amplitude_spectrum.begin() + max_index);
    }
}

// 获取Fourier相位谱
std::vector<double> GmpCalculation::FourierPhaseSpectrum()
{
    if (fourier_transform_result_.empty())
    {
        fourier_transform();
    }
    std::size_t max_index =
        static_cast<std::size_t>(parameter_.fourier_spectrum_max_frequency_
                                 / parameter_.fourier_spectrum_df_);
    std::vector<double> phase_spectrum(max_index);
    for (size_t i = 0; i != max_index; i++)
    {
        phase_spectrum[i] = std::arg(fourier_transform_result_[i]);
    }
    if (max_index == 0)
        return phase_spectrum;
    else
    {
        return std::vector<double>(phase_spectrum.begin(),
                                   phase_spectrum.begin() + max_index);
    }
}

// 获取功率谱
std::vector<double> GmpCalculation::PowerSpectrum()
{
    if (fourier_transform_result_.empty())
    {
        fourier_transform();
    }
    std::size_t max_index =
        static_cast<std::size_t>(parameter_.fourier_spectrum_max_frequency_
                                 / parameter_.fourier_spectrum_df_);
    std::vector<double> power_spectrum(max_index);
    for (size_t i = 0; i != max_index; i++)
    {
        power_spectrum[i] = std::norm(fourier_transform_result_[i]);
    }
    if (max_index == 0)
        return power_spectrum;
    else
    {
        return std::vector<double>(power_spectrum.begin(),
                                   power_spectrum.begin() + max_index);
    }
}

/** 功能函数 **/

// 初始化滤波器
void GmpCalculation::init_filter()
{
    // 初始化滤波器
    double low = parameter_.low_frequency_ / parameter_.frequency_ * 2,
           high = parameter_.high_frequency_ / parameter_.low_frequency_ * low;
    auto filter_generator = numerical_algorithm::ButterworthFilterDesign(
        parameter_.filter_order_, low, high, parameter_.filter_type_);
    filter_function_ = numerical_algorithm::Filter(filter_generator);
}

// 计算速度
void GmpCalculation::calculate_velocity()
{
    // 加速度积分到速度
    auto velocity =
        numerical_algorithm::Cumtrapz(acceleration_, parameter_.time_step_);
    // 是否初滤波
    if (parameter_.filter_)
    {
        // 速度信号滤波
        velocity_ = filter_function_.Filtering(velocity);
    }
    else
    {
        velocity_ = velocity;
    }
}

// 计算位移
void GmpCalculation::calculate_displacement()
{
    if (velocity_.empty())
    {
        calculate_velocity();
    }
    // 速度积分到位移
    auto displacement =
        numerical_algorithm::Cumtrapz(velocity_, parameter_.time_step_);
    // 是否滤波
    if (parameter_.filter_)
    {
        // 位移滤波
        displacement_ = filter_function_.Filtering(displacement);
    }
    else
    {
        displacement_ = displacement;
    }
}

// 计算Fourier变换
void GmpCalculation::fourier_transform()
{
    fourier_transform_result_.resize(acceleration_.size());
    fftw_plan plan = fftw_plan_dft_r2c_1d(
        acceleration_.size(),
        acceleration_.data(),
        reinterpret_cast<fftw_complex *>(fourier_transform_result_.data()),
        FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);

    parameter_.fourier_spectrum_df_ =
        parameter_.frequency_ / fourier_transform_result_.size();
}

// 清除已有计算结果
void GmpCalculation::clear_result()
{
    velocity_.clear();
    displacement_.clear();
    response_spectrum_.Sa.clear();
    response_spectrum_.Sv.clear();
    response_spectrum_.Sd.clear();
    pesudo_response_spectrum_.Sa.clear();
    pesudo_response_spectrum_.Sv.clear();
    pesudo_response_spectrum_.Sd.clear();
    response_spectrum_flag_ = false;
    pesudo_response_spectrum_flag_ = false;
    fourier_transform_result_.clear();
}

} // namespace gmp_calculation