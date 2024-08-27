/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\gmp_calculation\gmp_calculation.h
** -----
** File Created: Tuesday, 23rd July 2024 22:50:47
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Wednesday, 21st August 2024 20:04:29
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 计算地震参数的类。
// 已经完成了各种反应谱、持时（95%）、峰值信息、Fourier变换等计算。
// TODO: Arias强度、Housner强度、能量密度谱等计算有待加入。
// TODO: 反应谱的横轴最大值、步长的设置有待加入。

#ifndef GMP_CALCULATION_GMP_CACULATION_H
#define GMP_CALCULATION_GMP_CACULATION_H

#include <cstddef>
#define _USE_MATH_DEFINES
// stdc++ headers
#include <cmath>
#include <complex>
#include <memory>
#include <numeric>
#include <vector>


// third-party headers
#include "fftw3.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// project headers
#include <cstdlib>
#include "numerical_algorithm/integral.h"
#include "numerical_algorithm/vector_calculation.h"


namespace gmp_calculation
{
// 计算地震动参数需要参数的结构体
struct GmpCalculationParameter
{
    // 采样频率
    double frequency_ = 50;
    // 信号时间步长
    double time_step_ = 1.0 / frequency_;
    // 阻尼比
    double damping_ratio_ = 0.05;
    // 反应谱横轴周期的步长
    double response_spectrum_dt_ = 0.01;
    // 反应谱横轴最大周期
    double response_spectrum_max_period_ = 5;
    // Fourier横轴频率间隔（无需设置，仅作读取用）
    double fourier_spectrum_df_ = 0;
    // Fourier谱横轴最大频率，0表示不限制
    double fourier_spectrum_max_frequency_ = 10;
}; // struct GmpCalculationParameter

// 地震反应谱计算结果
struct ResponseSpectrumResult
{
    std::vector<double> Sa{};
    std::vector<double> Sv{};
    std::vector<double> Sd{};
};

// Ti周期下地震反应谱计算结果
struct ResponseSpectrumTiResult
{
    double SaTi{};
    double SvTi{};
    double SdTi{};
};

// 地震参数计算类
class GmpCalculation
{
public:
    // 默认构造函数
    GmpCalculation() = default;

    // 从std::vector构造
    // @param acceleration 加速度数据
    // @param frequency 频率
    // @param damping_ratio 阻尼比
    inline explicit GmpCalculation(const std::vector<double> &acceleration,
                                   double frequency,
                                   double damping_ratio = 0.05);

    // 从std::vector指针构造
    // @param acceleration_ptr 加速度数据指针
    // @param frequency 频率
    // @param damping_ratio 阻尼比
    inline explicit GmpCalculation(
        const std::shared_ptr<std::vector<double>> &acceleration_ptr,
        double frequency,
        double damping_ratio = 0.05);

    // 从配置文件中读取参数构造
    // @param acceleration 加速度数据
    // @param config_file 配置文件路径
    explicit GmpCalculation(const std::vector<double> &acceleration);

    // 析构函数
    ~GmpCalculation() = default;

private:
    // 加速度数据指针
    std::shared_ptr<std::vector<double>> acceleration_ptr_ = nullptr;
    // GMP计算参数
    GmpCalculationParameter parameter_{};
    // 速度和位移（根据传入的加速度计算）
    std::vector<double> velocity_{}, displacement_{};

    /** 各种反应谱计算结果 **/
    // 反应谱计算结果
    ResponseSpectrumResult response_spectrum_{};
    // 反应谱在Ti的计算结果
    ResponseSpectrumTiResult response_spectrum_ti_{};
    // 拟反应谱计算结果
    ResponseSpectrumResult pesudo_response_spectrum_{};
    // 拟反应谱在Ti的计算结果
    ResponseSpectrumTiResult pesudo_response_spectrum_ti_{};
    // 是否已经计算了反应谱
    bool response_spectrum_flag_{false};
    // 是否已经计算了拟反应谱
    bool pesudo_response_spectrum_flag_{false};

    // Fourier变换结果
    std::vector<std::complex<double>> fourier_transform_result_;

    /** 功能函数 **/

    // 计算速度
    inline void calculate_velocity();

    // 计算位移
    inline void calculate_displacement();

    // NewmakeBeta方法计算响应
    struct ResponseSpectrumTiResult NewmarkBeta(const double &Ti);

    // 计算Fourier变换
    inline void fourier_transform();

    // 清除已有计算结果
    inline void clear_result();

public:
    /** 读取和设置参数 **/

    // 设置加速度数据
    // @param acceleration 加速度数据
    inline void set_acceleration(const std::vector<double> &acceleration);

    // 设置频率
    // @param frequency 频率
    inline void set_frequency(double frequency);

    // 设置阻尼比
    // @param damping_ratio 阻尼比
    inline void set_damping_ratio(double damping_ratio);

    // 从配置文件中读取参数
    // @param config_file 配置文件路径
    void LoadConfig(const std::string &config_file = "config/GMP_Config.json");

    // 获取计算参数
    const GmpCalculationParameter &get_parameter() const { return parameter_; }

    // 获取加速度数据
    // @return 加速度数据
    inline std::vector<double> &get_acceleration();

    // 获取频率
    // @return 频率
    inline double get_frequency() const;

    // 获取阻尼比
    // @return 阻尼比
    inline double get_damping_ratio() const;

    // 获取速度
    // @return 速度
    inline std::vector<double> get_velocity();

    // 获取位移
    // @return 位移
    inline std::vector<double> get_displacement();

    // 获取反应谱计算结果
    // @return 反应谱计算结果
    inline ResponseSpectrumResult get_response_spectrum();

    // 获取拟反应谱计算结果
    // @return 拟反应谱计算结果
    inline ResponseSpectrumResult get_pseudo_response_spectrum();

    /** 峰值信息 */

    // 获取峰值加速度
    // @return 峰值加速度
    inline double PeakAcceleration() const;

    // 获取峰值速度
    // @return 峰值速度
    inline double PeakVelocity();

    // 获取峰值位移
    // @return 峰值位移
    inline double PeakDisplacement();

    // 获取加速度均方根
    // @return 加速度均方根
    inline double RmsAcceleration();

    // 获取速度均方根
    // @return 速度均方根
    inline double RmsVelocity();

    // 获取位移均方根
    // @return 位移均方根
    inline double RmsDisplacement();

    /** 持时信息 **/

    // 获取持时，定义为加速度绝对值大于峰值加速度5%的时间间隔
    // @return 持时
    inline double Duration();

    /** 地震反应谱 **/

    // 获取Ti周期下反应谱谱值
    inline ResponseSpectrumTiResult ResponseSpectrumTi(const double &Ti);

    // 获取Ti周期下加速度反应谱谱值
    inline double AccelerationSpectrumTi(const double &Ti);

    // 获取Ti周期下速度反应谱谱值
    inline double VelocitySpectrumTi(const double &Ti);

    // 获取Ti周期下位移反应谱谱值
    inline double DisplacementSpectrumTi(const double &Ti);

    // 获取反应谱
    inline ResponseSpectrumResult ResponseSpectrum();

    // 获取加速度反应谱
    inline std::vector<double> AccelerationSpectrum();

    // 获取速度反应谱
    inline std::vector<double> VelocitySpectrum();

    // 获取位移反应谱
    inline std::vector<double> DisplacementSpectrum();

    // 获取Ti周期下拟反应谱谱值
    inline ResponseSpectrumTiResult PseudoResponseSpectrumTi(const double &Ti);

    // 获取Ti周期下拟加速度反应谱谱值
    inline double PseudoAccelerationSpectrumTi(const double &Ti);

    // 获取Ti周期下拟速度反应谱谱值
    inline double PseudoVelocitySpectrumTi(const double &Ti);

    // 获取拟反应谱
    inline ResponseSpectrumResult PseudoResponseSpectrum();

    // 获取拟加速度反应谱
    inline std::vector<double> PseudoAccelerationSpectrum();

    // 获取拟速度反应谱
    inline std::vector<double> PseudoVelocitySpectrum();

    /** 频域信息 **/

    // 获取Fourier幅值谱
    inline std::vector<double> FourierAmplitudeSpectrum();

    // 获取Fourier相位谱
    inline std::vector<double> FourierPhaseSpectrum();

    // 获取功率谱
    inline std::vector<double> PowerSpectrum();
};

/** 读取和设置参数 **/

// 设置加速度数据
inline void
GmpCalculation::set_acceleration(const std::vector<double> &acceleration)
{
    acceleration_ptr_ = std::make_shared<std::vector<double>>(acceleration);
    clear_result();
}

// 设置频率
inline void GmpCalculation::set_frequency(double frequency)
{
    parameter_.frequency_ = frequency;
    parameter_.time_step_ = 1.0 / frequency;
    clear_result();
}

// 设置阻尼比
inline void GmpCalculation::set_damping_ratio(double damping_ratio)
{
    parameter_.damping_ratio_ = damping_ratio;
    clear_result();
}

// 获取加速度数据
inline std::vector<double> &GmpCalculation::get_acceleration()
{
    return *acceleration_ptr_;
}

// 获取频率
inline double GmpCalculation::get_frequency() const
{
    return parameter_.frequency_;
}

// 获取阻尼比
inline double GmpCalculation::get_damping_ratio() const
{
    return parameter_.damping_ratio_;
}

// 获取速度
inline std::vector<double> GmpCalculation::get_velocity()
{
    if (velocity_.empty())
    {
        calculate_velocity();
    }
    return velocity_;
}

// 获取位移
inline std::vector<double> GmpCalculation::get_displacement()
{
    if (displacement_.empty())
    {
        calculate_displacement();
    }
    return displacement_;
}

// 获取反应谱计算结果
inline ResponseSpectrumResult GmpCalculation::get_response_spectrum()
{
    if (!response_spectrum_flag_)
    {
        ResponseSpectrum();
    }
    return response_spectrum_;
}

// 获取拟反应谱计算结果
inline ResponseSpectrumResult GmpCalculation::get_pseudo_response_spectrum()
{
    if (!pesudo_response_spectrum_flag_)
    {
        PseudoResponseSpectrum();
    }
    return pesudo_response_spectrum_;
}

/** 峰值信息 */

// 获取峰值加速度
inline double GmpCalculation::PeakAcceleration() const
{
    return numerical_algorithm::FindMaxAbs(*acceleration_ptr_);
}

// 获取峰值速度
inline double GmpCalculation::PeakVelocity()
{
    if (velocity_.empty())
    {
        calculate_velocity();
    }
    return numerical_algorithm::FindMaxAbs(velocity_);
}

// 获取峰值位移
inline double GmpCalculation::PeakDisplacement()
{
    if (displacement_.empty())
    {
        calculate_displacement();
    }
    return numerical_algorithm::FindMaxAbs(displacement_);
}

// 获取加速度均方根
inline double GmpCalculation::RmsAcceleration()
{
    double sum = std::accumulate(acceleration_ptr_->begin(),
                                 acceleration_ptr_->end(),
                                 0.0,
                                 [](double a, double b) { return a + b * b; });
    return std::sqrt(sum / acceleration_ptr_->size());
}

// 获取速度均方根
inline double GmpCalculation::RmsVelocity()
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
inline double GmpCalculation::RmsDisplacement()
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
inline double GmpCalculation::Duration()
{
    double peak_acceleration = PeakAcceleration();
    auto it1 = std::find_if(acceleration_ptr_->begin(),
                            acceleration_ptr_->end(),
                            [peak_acceleration](double a) {
                                return std::abs(a) > 0.05 * peak_acceleration;
                            });
    auto it2 = std::find_if(acceleration_ptr_->rbegin(),
                            acceleration_ptr_->rend(),
                            [peak_acceleration](double a) {
                                return std::abs(a) > 0.05 * peak_acceleration;
                            });
    return std::distance(it1, it2.base()) * parameter_.time_step_;
}

/** 地震反应谱 **/

// 获取Ti周期下反应谱谱值
inline ResponseSpectrumTiResult
GmpCalculation::ResponseSpectrumTi(const double &Ti)
{
    return NewmarkBeta(Ti);
}

// 获取Ti周期下加速度反应谱谱值
inline double GmpCalculation::AccelerationSpectrumTi(const double &Ti)
{
    return NewmarkBeta(Ti).SaTi;
}

// 获取Ti周期下速度反应谱谱值
inline double GmpCalculation::VelocitySpectrumTi(const double &Ti)
{
    return NewmarkBeta(Ti).SvTi;
}

// 获取Ti周期下位移反应谱谱值
inline double GmpCalculation::DisplacementSpectrumTi(const double &Ti)
{
    return NewmarkBeta(Ti).SdTi;
}

// 获取反应谱
inline ResponseSpectrumResult GmpCalculation::ResponseSpectrum()
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
inline std::vector<double> GmpCalculation::AccelerationSpectrum()
{
    if (!response_spectrum_flag_)
    {
        ResponseSpectrum();
    }
    return response_spectrum_.Sa;
}

// 获取速度反应谱
inline std::vector<double> GmpCalculation::VelocitySpectrum()
{
    if (!response_spectrum_flag_)
    {
        ResponseSpectrum();
    }
    return response_spectrum_.Sv;
}

// 获取位移反应谱
inline std::vector<double> GmpCalculation::DisplacementSpectrum()
{
    if (!response_spectrum_flag_)
    {
        ResponseSpectrum();
    }
    return response_spectrum_.Sd;
}

// 获取Ti周期下拟反应谱谱值
inline ResponseSpectrumTiResult
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
inline double GmpCalculation::PseudoAccelerationSpectrumTi(const double &Ti)
{
    const double omega = 2 * M_PI / Ti;
    return ResponseSpectrumTi(Ti).SaTi * omega * omega;
}

// 获取Ti周期下拟速度反应谱谱值
inline double GmpCalculation::PseudoVelocitySpectrumTi(const double &Ti)
{
    const double omega = 2 * M_PI / Ti;
    return ResponseSpectrumTi(Ti).SvTi * omega;
}

// 获取拟反应谱
inline ResponseSpectrumResult GmpCalculation::PseudoResponseSpectrum()
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
inline std::vector<double> GmpCalculation::PseudoAccelerationSpectrum()
{
    if (!pesudo_response_spectrum_flag_)
    {
        PseudoResponseSpectrum();
    }
    return pesudo_response_spectrum_.Sa;
}

// 获取拟速度反应谱
inline std::vector<double> GmpCalculation::PseudoVelocitySpectrum()
{
    if (!pesudo_response_spectrum_flag_)
    {
        PseudoResponseSpectrum();
    }
    return pesudo_response_spectrum_.Sv;
}

/** 频域信息 **/

// 获取Fourier幅值谱
inline std::vector<double> GmpCalculation::FourierAmplitudeSpectrum()
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
inline std::vector<double> GmpCalculation::FourierPhaseSpectrum()
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
inline std::vector<double> GmpCalculation::PowerSpectrum()
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

// 计算速度
inline void GmpCalculation::calculate_velocity()
{
    velocity_ = numerical_algorithm::Cumtrapz(*acceleration_ptr_,
                                              parameter_.time_step_);
}

// 计算位移
inline void GmpCalculation::calculate_displacement()
{
    if (velocity_.empty())
    {
        calculate_velocity();
    }
    displacement_ =
        numerical_algorithm::Cumtrapz(velocity_, parameter_.time_step_);
}

// 计算Fourier变换
inline void GmpCalculation::fourier_transform()
{
    fourier_transform_result_.resize(acceleration_ptr_->size());
    fftw_plan plan = fftw_plan_dft_r2c_1d(
        acceleration_ptr_->size(),
        acceleration_ptr_->data(),
        reinterpret_cast<fftw_complex *>(fourier_transform_result_.data()),
        FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);

    parameter_.fourier_spectrum_df_ =
        parameter_.frequency_ / fourier_transform_result_.size();
}

// 清除已有计算结果
inline void GmpCalculation::clear_result()
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

#endif // GMP_CALCULATION_GMP_CACULATION_H