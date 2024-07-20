/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/gmp_calculation/gmp_caculation.h
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
// Description:
// 计算地震参数的类。

#ifndef GMP_CALCULATION_GMP_CACULATION_H
#define GMP_CALCULATION_GMP_CACULATION_H

#define _USE_MATH_DEFINES
// stdc++ headers
#include <algorithm>
#include <cmath>
#include <complex>
#include <memory>
#include <tuple>
#include <vector>


// third-party headers
#include "fftw3.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// project headers
#include "data_structure/acceleration.h"
#include "numerical_algorithm/integral.h"
#include "numerical_algorithm/vector_calculation.h"


namespace gmp_calculation
{
// 地震反应谱计算结果
struct ResponseSpectrum
{
    ResponseSpectrum() = default;
    ResponseSpectrum(const std::vector<double> &Sa,
                     const std::vector<double> &Sv,
                     const std::vector<double> &Sd)
        : Sa(Sa), Sv(Sv), Sd(Sd) {};

    std::vector<double> Sa{};
    std::vector<double> Sv{};
    std::vector<double> Sd{};
};

// Ti周期下地震反应谱计算结果
struct ResponseSpectrumTi
{
    ResponseSpectrumTi() = default;
    ResponseSpectrumTi(const double &SaTi,
                       const double &SvTi,
                       const double &SdTi)
        : SaTi(SaTi), SvTi(SvTi), SdTi(SdTi) {};
    double SaTi{};
    double SvTi{};
    double SdTi{};
};

// 地震拟反应谱计算结果
struct PseudoResponseSpectrum
{
    PseudoResponseSpectrum() = default;
    PseudoResponseSpectrum(const std::vector<double> &PsSa,
                           const std::vector<double> &PsSv,
                           const std::vector<double> &PsSd)
        : PsSa(PsSa), PsSv(PsSv), PsSd(PsSd) {};
    std::vector<double> PsSa{};
    std::vector<double> PsSv{};
    std::vector<double> PsSd{};
};

// Ti周期下地震拟反应谱计算结果
struct PseudoResponseSpectrumTi
{
    PseudoResponseSpectrumTi() = default;
    PseudoResponseSpectrumTi(const double &PsSaTi,
                             const double &PsSvTi,
                             const double &PsSdTi)
        : PsSaTi(PsSaTi), PsSvTi(PsSvTi), PsSdTi(PsSdTi) {};
    double PsSaTi{};
    double PsSvTi{};
    double PsSdTi{};
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

    // 从加速度信息类构造
    // @param acceleration 加速度信息类变量
    // @param damping_ratio 阻尼比
    explicit GmpCalculation(const data_structure::Acceleration &acceleration,
                            double damping_ratio = 0.05);

    // 拷贝构造函数
    GmpCalculation(const GmpCalculation &other) = default;

    // 移动构造函数
    GmpCalculation(GmpCalculation &&other) = default;

    // 析构函数
    ~GmpCalculation() = default;

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
    inline ResponseSpectrumTi ResponseSpectrumTi(const double &Ti);

    // 获取Ti周期下加速度反应谱谱值
    inline double AccelerationSpectrumTi(const double &Ti);

    // 获取Ti周期下速度反应谱谱值
    inline double VelocitySpectrumTi(const double &Ti);

    // 获取Ti周期下位移反应谱谱值
    inline double DisplacementSpectrumTi(const double &Ti);

    // 获取反应谱
    inline ResponseSpectrum ResponseSpectrum();

    // 获取加速度反应谱
    inline std::vector<double> AccelerationSpectrum();

    // 获取速度反应谱
    inline std::vector<double> VelocitySpectrum();

    // 获取位移反应谱
    inline std::vector<double> DisplacementSpectrum();

    // 获取Ti周期下拟反应谱谱值
    inline PseudoResponseSpectrumTi PseudoResponseSpectrumTi(const double &Ti);

    // 获取Ti周期下拟加速度反应谱谱值
    inline double PseudoAccelerationSpectrumTi(const double &Ti);

    // 获取Ti周期下拟速度反应谱谱值
    inline double PseudoVelocitySpectrumTi(const double &Ti);

    // 获取拟反应谱
    inline PseudoResponseSpectrum PseudoResponseSpectrum();

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

private:
    // 加速度数据指针
    std::shared_ptr<std::vector<double>> acceleration_ptr_ = nullptr;
    // 频率和阻尼比
    double frequency_{}, time_step_{}, damping_ratio_{};
    // 速度和位移（根据传入的加速度计算）
    std::vector<double> velocity_{}, displacement_{};

    // 反应谱计算结果
    struct ResponseSpectrum response_spectrum_;
    // 反应谱在Ti的计算结果
    struct ResponseSpectrumTi response_spectrum_ti_;
    // 拟反应谱计算结果
    struct PseudoResponseSpectrum pesudo_response_spectrum_;
    // 拟反应谱在Ti的计算结果
    struct PseudoResponseSpectrumTi pesudo_response_spectrum_ti_;
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
    struct ResponseSpectrumTi NewmarkBeta(const double &Ti);

    // 计算Fourier变换
    inline void fourier_transform();

    // 清除已有计算结果
    inline void clear_result();
};

/** 构造函数 **/

// 从std::vector构造
inline GmpCalculation::GmpCalculation(const std::vector<double> &acceleration,
                                      double frequency,
                                      double damping_ratio)
    : acceleration_ptr_(std::make_shared<std::vector<double>>(acceleration)),
      frequency_(frequency),
      time_step_(1.0 / frequency),
      damping_ratio_(damping_ratio)
{}

// 从std::vector指针构造
inline GmpCalculation::GmpCalculation(
    const std::shared_ptr<std::vector<double>> &acceleration_ptr,
    double frequency,
    double damping_ratio)
    : acceleration_ptr_(acceleration_ptr),
      frequency_(frequency),
      time_step_(1.0 / frequency),
      damping_ratio_(damping_ratio)
{}

// 从加速度信息类构造
inline GmpCalculation::GmpCalculation(
    const data_structure::Acceleration &acceleration,
    double damping_ratio)
    : acceleration_ptr_(
          std::make_shared<std::vector<double>>(acceleration.get_data())),
      frequency_(acceleration.get_frequency()),
      time_step_(1.0 / frequency_),
      damping_ratio_(damping_ratio)
{}

/** 读取和设置参数 **/

// 设置加速度数据
// @param acceleration 加速度数据
inline void
GmpCalculation::set_acceleration(const std::vector<double> &acceleration)
{
    acceleration_ptr_ = std::make_shared<std::vector<double>>(acceleration);
    clear_result();
}

// 设置频率
// @param frequency 频率
inline void GmpCalculation::set_frequency(double frequency)
{
    frequency_ = frequency;
    time_step_ = 1.0 / frequency;
    clear_result();
}

// 设置阻尼比
// @param damping_ratio 阻尼比
inline void GmpCalculation::set_damping_ratio(double damping_ratio)
{
    damping_ratio_ = damping_ratio;
    clear_result();
}

// 获取加速度数据
// @return 加速度数据
inline std::vector<double> &GmpCalculation::get_acceleration()
{
    return *acceleration_ptr_;
}

// 获取频率
// @return 频率
inline double GmpCalculation::get_frequency() const { return frequency_; }

// 获取阻尼比
// @return 阻尼比
inline double GmpCalculation::get_damping_ratio() const
{
    return damping_ratio_;
}

// 获取速度
// @return 速度
inline std::vector<double> GmpCalculation::get_velocity()
{
    if (velocity_.empty())
    {
        calculate_velocity();
    }
    return velocity_;
}

// 获取位移
// @return 位移
inline std::vector<double> GmpCalculation::get_displacement()
{
    if (displacement_.empty())
    {
        calculate_displacement();
    }
    return displacement_;
}

/** 峰值信息 */

// 获取峰值加速度
// @return 峰值加速度
inline double GmpCalculation::PeakAcceleration() const
{
    return numerical_algorithm::FindMaxAbs(*acceleration_ptr_);
}

// 获取峰值速度
// @return 峰值速度
inline double GmpCalculation::PeakVelocity()
{
    if (velocity_.empty())
    {
        calculate_velocity();
    }
    return numerical_algorithm::FindMaxAbs(velocity_);
}

// 获取峰值位移
// @return 峰值位移
inline double GmpCalculation::PeakDisplacement()
{
    if (displacement_.empty())
    {
        calculate_displacement();
    }
    return numerical_algorithm::FindMaxAbs(displacement_);
}

// 获取加速度均方根
// @return 加速度均方根
inline double GmpCalculation::RmsAcceleration()
{
    double sum = std::accumulate(acceleration_ptr_->begin(),
                                 acceleration_ptr_->end(),
                                 0.0,
                                 [](double a, double b) { return a + b * b; });
    return std::sqrt(sum / acceleration_ptr_->size());
}

// 获取速度均方根
// @return 速度均方根
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
// @return 位移均方根
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
// @return 持时
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
    return std::distance(it1, it2.base()) * time_step_;
}

/** 地震反应谱 **/

// 获取Ti周期下反应谱谱值
inline ResponseSpectrumTi GmpCalculation::ResponseSpectrumTi(const double &Ti)
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
inline ResponseSpectrum GmpCalculation::ResponseSpectrum()
{
    clear_result();
    for (double Ti = 0.01; Ti <= 5; Ti += 0.01)
    {
        NewmarkBeta(Ti);
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
inline PseudoResponseSpectrumTi
GmpCalculation::PseudoResponseSpectrumTi(const double &Ti)
{
    ResponseSpectrumTi(Ti);

    const double omega = 2 * M_PI / Ti;
    pesudo_response_spectrum_ti_.PsSaTi =
        response_spectrum_ti_.SaTi * omega * omega;
    pesudo_response_spectrum_ti_.PsSvTi = response_spectrum_ti_.SvTi * omega;
    pesudo_response_spectrum_ti_.PsSdTi = response_spectrum_ti_.SdTi;
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
inline PseudoResponseSpectrum GmpCalculation::PseudoResponseSpectrum()
{
    double omega;
    for (double Ti = 0.01; Ti <= 5; Ti += 0.01)
    {
        omega = 2 * M_PI / Ti;
        if (!response_spectrum_flag_)
        {
            ResponseSpectrum();
        }
        pesudo_response_spectrum_.PsSa.push_back(response_spectrum_ti_.SaTi
                                                 * omega * omega);
        pesudo_response_spectrum_.PsSv.push_back(response_spectrum_ti_.SvTi
                                                 * omega);
        pesudo_response_spectrum_.PsSd.push_back(response_spectrum_ti_.SdTi);
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
    return pesudo_response_spectrum_.PsSa;
}

// 获取拟速度反应谱
inline std::vector<double> GmpCalculation::PseudoVelocitySpectrum()
{
    if (!pesudo_response_spectrum_flag_)
    {
        PseudoResponseSpectrum();
    }
    return pesudo_response_spectrum_.PsSv;
}

/** 频域信息 **/

// 获取Fourier幅值谱
inline std::vector<double> GmpCalculation::FourierAmplitudeSpectrum()
{
    if (fourier_transform_result_.empty())
    {
        fourier_transform();
    }
    std::vector<double> amplitude_spectrum(fourier_transform_result_.size());
    for (size_t i = 0; i != fourier_transform_result_.size(); i++)
    {
        amplitude_spectrum[i] = std::abs(fourier_transform_result_[i]);
    }
    return amplitude_spectrum;
}

// 获取Fourier相位谱
inline std::vector<double> GmpCalculation::FourierPhaseSpectrum()
{
    if (fourier_transform_result_.empty())
    {
        fourier_transform();
    }
    std::vector<double> phase_spectrum(fourier_transform_result_.size());
    for (size_t i = 0; i != fourier_transform_result_.size(); i++)
    {
        phase_spectrum[i] = std::arg(fourier_transform_result_[i]);
    }
    return phase_spectrum;
}

// 获取功率谱
inline std::vector<double> GmpCalculation::PowerSpectrum()
{
    if (fourier_transform_result_.empty())
    {
        fourier_transform();
    }
    std::vector<double> power_spectrum(fourier_transform_result_.size());
    for (size_t i = 0; i != fourier_transform_result_.size(); i++)
    {
        power_spectrum[i] = std::norm(fourier_transform_result_[i]);
    }
    return power_spectrum;
}

/** 功能函数 **/

// 计算速度
inline void GmpCalculation::calculate_velocity()
{
    velocity_ = numerical_algorithm::Cumtrapz(*acceleration_ptr_, time_step_);
}

// 计算位移
inline void GmpCalculation::calculate_displacement()
{
    if (velocity_.empty())
    {
        calculate_velocity();
    }
    displacement_ = numerical_algorithm::Cumtrapz(velocity_, time_step_);
}

// 计算Fourier变换
inline void GmpCalculation::fourier_transform()
{
    fftw_plan plan = fftw_plan_dft_r2c_1d(
        acceleration_ptr_->size(),
        acceleration_ptr_->data(),
        reinterpret_cast<fftw_complex *>(fourier_transform_result_.data()),
        FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
}

// 清除已有计算结果
inline void GmpCalculation::clear_result()
{
    velocity_.clear();
    displacement_.clear();
    response_spectrum_.Sa.clear();
    response_spectrum_.Sv.clear();
    response_spectrum_.Sd.clear();
    pesudo_response_spectrum_.PsSa.clear();
    pesudo_response_spectrum_.PsSv.clear();
    pesudo_response_spectrum_.PsSd.clear();
    response_spectrum_flag_ = false;
    pesudo_response_spectrum_flag_ = false;
    fourier_transform_result_.clear();
}

} // namespace gmp_calculation

#endif // GMP_CALCULATION_GMP_CACULATION_H