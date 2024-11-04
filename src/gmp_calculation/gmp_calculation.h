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

// Description: 地震动参数计算基类。
// TODO: Arias强度、Housner强度、能量密度谱等计算有待加入。
// TODO: 反应谱的横轴最大值、步长的设置有待加入。

#ifndef GMP_CALCULATION_GMP_CACULATION_H
#define GMP_CALCULATION_GMP_CACULATION_H

#include <cstddef>
#define _USE_MATH_DEFINES
// stdc++ headers
#include <cmath>
#include <complex>
#include <cstdlib>
#include <memory>
#include <numeric>
#include <vector>

// third-party headers
#include "fftw3.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// project headers
#include "numerical_algorithm/filter.h"
#include "numerical_algorithm/integral.h"
#include "numerical_algorithm/vector_calculation.h"
#include "settings/config.h"

namespace gmp_calculation
{
// 地震（拟）反应谱计算结果
struct ResponseSpectrumResult
{
    std::vector<double> Sa{};
    std::vector<double> Sv{};
    std::vector<double> Sd{};
}; // struct ResponseSpectrumResult

// Ti周期下地震（拟）反应谱计算结果
struct ResponseSpectrumTiResult
{
    double SaTi{};
    double SvTi{};
    double SdTi{};
}; // struct ResponseSpectrumTiResult


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

    // 是否滤波
    bool filter_ = true;
    // 滤波器阶数
    int filter_order_ = 4;
    // 滤波器截止频率
    double low_frequency_ = 0.1, high_frequency_ = 20;
    // 滤波器类型：
    // lowpass：低通滤波；
    // highpass：高通滤波；
    // *bandpass：带通滤波；
    numerical_algorithm::FilterType filter_type_ =
        numerical_algorithm::FilterType::bandpass;
    // 滤波函数：
    // filter：单向滤波；
    // *filtfilt：零相位双向滤波
    numerical_algorithm::FilterFunction filter_function_ =
        numerical_algorithm::FilterFunction::filter;
    // 滤波器类型：
    // *butter：巴特沃斯滤波器；
    numerical_algorithm::FilterGenerator filter_generator_ =
        numerical_algorithm::FilterGenerator::butter;
}; // struct GmpCalculationParameter

// 地震参数计算类
class GmpCalculation
{
public:
    // 默认构造函数
    GmpCalculation() = default;

    // 从std::vector构造
    // @param acceleration 加速度数据
    // @param config 配置信息
    GmpCalculation(const std::vector<double> &acceleration,
                   std::shared_ptr<settings::Config> config);

    // 从std::vector指针构造
    // @param acceleration_ptr 加速度数据指针
    // @param config 配置信息
    GmpCalculation(const std::shared_ptr<std::vector<double>> &acceleration_ptr,
                   std::shared_ptr<settings::Config> config);

    // 析构函数
    ~GmpCalculation() = default;

    // 从配置文件中读取参数
    // @param config 配置信息
    void LoadConfig(std::shared_ptr<settings::Config> config);

private:
    // GMP计算参数
    GmpCalculationParameter parameter_{};
    // 原始加速度信号
    std::vector<double> ori_acceleration_{};
    // 加速度、速度和位移（根据传入的加速度计算）
    std::vector<double> acceleration_{}, velocity_{}, displacement_{};

    // 滤波类
    numerical_algorithm::Filter filter_function_{};

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

    // 初始化滤波器
    void init_filter();

    // 计算速度
    void calculate_velocity();

    // 计算位移
    void calculate_displacement();

    // NewmakeBeta方法计算响应
    struct ResponseSpectrumTiResult NewmarkBeta(const double &Ti);

    // 计算Fourier变换
    void fourier_transform();

    // 清除已有计算结果
    void clear_result();

public:
    /** 读取和设置参数 **/

    // 设置加速度数据
    // @param acceleration 加速度数据
    void set_acceleration(const std::vector<double> &acceleration);

    // 设置频率
    // @param frequency 频率
    void set_frequency(double frequency);

    // 设置阻尼比
    // @param damping_ratio 阻尼比
    void set_damping_ratio(double damping_ratio);

    // 获取计算参数，如果更新了参数，记得调用Update()更新
    // @return 计算参数的引用
    GmpCalculationParameter &get_parameter() { return parameter_; }

    // 更改参数后更新相关参数
    void Update();

    // 获取加速度数据
    // @return 加速度数据
    std::vector<double> &get_acceleration();

    // 获取频率
    // @return 频率
    double get_frequency() const;

    // 获取阻尼比
    // @return 阻尼比
    double get_damping_ratio() const;

    // 获取速度
    // @return 速度
    std::vector<double> get_velocity();

    // 获取位移
    // @return 位移
    std::vector<double> get_displacement();

    // 获取反应谱计算结果
    // @return 反应谱计算结果
    ResponseSpectrumResult get_response_spectrum();

    // 获取拟反应谱计算结果
    // @return 拟反应谱计算结果
    ResponseSpectrumResult get_pseudo_response_spectrum();

    /** 峰值信息 */

    // 获取峰值加速度
    // @return 峰值加速度
    double PeakAcceleration() const;

    // 获取峰值速度
    // @return 峰值速度
    double PeakVelocity();

    // 获取峰值位移
    // @return 峰值位移
    double PeakDisplacement();

    // 获取加速度均方根
    // @return 加速度均方根
    double RmsAcceleration();

    // 获取速度均方根
    // @return 速度均方根
    double RmsVelocity();

    // 获取位移均方根
    // @return 位移均方根
    double RmsDisplacement();

    /** 持时信息 **/

    // 获取持时，定义为加速度绝对值大于峰值加速度5%的时间间隔
    // @return 持时
    double Duration();

    /** 地震反应谱 **/

    // 获取Ti周期下反应谱谱值
    ResponseSpectrumTiResult ResponseSpectrumTi(const double &Ti);

    // 获取Ti周期下加速度反应谱谱值
    double AccelerationSpectrumTi(const double &Ti);

    // 获取Ti周期下速度反应谱谱值
    double VelocitySpectrumTi(const double &Ti);

    // 获取Ti周期下位移反应谱谱值
    double DisplacementSpectrumTi(const double &Ti);

    // 获取反应谱
    ResponseSpectrumResult ResponseSpectrum();

    // 获取加速度反应谱
    std::vector<double> AccelerationSpectrum();

    // 获取速度反应谱
    std::vector<double> VelocitySpectrum();

    // 获取位移反应谱
    std::vector<double> DisplacementSpectrum();

    // 获取Ti周期下拟反应谱谱值
    ResponseSpectrumTiResult PseudoResponseSpectrumTi(const double &Ti);

    // 获取Ti周期下拟加速度反应谱谱值
    double PseudoAccelerationSpectrumTi(const double &Ti);

    // 获取Ti周期下拟速度反应谱谱值
    double PseudoVelocitySpectrumTi(const double &Ti);

    // 获取拟反应谱
    ResponseSpectrumResult PseudoResponseSpectrum();

    // 获取拟加速度反应谱
    std::vector<double> PseudoAccelerationSpectrum();

    // 获取拟速度反应谱
    std::vector<double> PseudoVelocitySpectrum();

    /** 频域信息 **/

    // 获取Fourier幅值谱
    std::vector<double> FourierAmplitudeSpectrum();

    // 获取Fourier相位谱
    std::vector<double> FourierPhaseSpectrum();

    // 获取功率谱
    std::vector<double> PowerSpectrum();
};

} // namespace gmp_calculation

#endif // GMP_CALCULATION_GMP_CACULATION_H