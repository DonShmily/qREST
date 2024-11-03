/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\gmp_library\gmp_library.h
** -----
** File Created: Monday, 5th August 2024 17:40:31
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Friday, 1st November 2024 16:25:26
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 地震动参数计算库(Extern "C")

#ifndef GMP_LIBRARY_H_
#define GMP_LIBRARY_H_

#ifdef __cplusplus
extern "C"
{
#endif

    // 反应谱参数计算参数
    typedef struct
    {
        // 反应谱横轴最大周期（s），默认5s
        double max_period = 5;
        // 反应谱横轴周期步长（s），默认0.02s
        double period_step = 0.02;
        // 阻尼比，默认0.05
        double damping_ratio = 0.05;
    } ResponseSpectrumConfig;

    // FFT计算参数
    typedef struct
    {
        // FFT窗口大小，0表示默认和原始信号长度相同
        int fft_size = 0;
    } FourierSpectrumConfig;

    // 反应谱计算结果
    typedef struct
    {
        // 反应谱计算结果
        double *Sa;
        double *Sv;
        double *Sd;
        double *period;

        // 计算结果长度，Sa、Sv、Sd、period的长度
        int result_size;

        // 计算参数
        ResponseSpectrumConfig *config;
    } ResponseSpectrum;

    // FFT计算结果
    typedef struct
    {
        // FFT计算结果
        double *fft_spectrum;
        double *fft_frequency;

        // FFT计算结果长度
        int result_size;

        // 计算参数
        FourierSpectrumConfig *config;
    } FourierSpectrum;

    // 计算反应谱
    // @param acceleration: 加速度数据
    // @param size: 加速度数据长度
    // @param frequency: 采样频率
    // @param damping_ratio: 阻尼比
    // @return ResponseSpectrum: 反应谱计算结果
    __declspec(dllexport) ResponseSpectrum *
    GetResponseSpectrum(const double *acceleration,
                        int size,
                        double frequency,
                        double damping_ratio);

    // 计算拟反应谱
    // @param acceleration: 加速度数据
    // @param size: 加速度数据长度
    // @param frequency: 采样频率
    // @param damping_ratio: 阻尼比
    // @return ResponseSpectrum: 反应谱计算结果
    __declspec(dllexport) ResponseSpectrum *
    GetPseudoResponseSpectrum(const double *acceleration,
                              int size,
                              double frequency,
                              double damping_ratio);

    // 释放反应谱结果内存
    // @param memory: 反应谱结果
    __declspec(dllexport) void FreeResponseSpectrum(ResponseSpectrum *memory);

    // 计算Fourier幅值谱
    // @param acceleration: 加速度数据
    // @param size: 加速度数据长度
    // @return double*: Fourier幅值谱
    __declspec(dllexport) FourierSpectrum *
    GetFourierSpectrum(const double *acceleration, int size, double frequency);

    // 释放Fourier幅值谱内存
    // @param memory: Fourier幅值谱
    __declspec(dllexport) void FreeFourierSpectrum(double *memory);

    // 获取反应谱计算参数
    // @param config_file: 计算参数文件
    // @return ResponseSpectrumConfig: 计算参数
    __declspec(dllexport) ResponseSpectrumConfig *
    GetResponseSpectrumConfig(const char *config_file);

    // 释放计算参数内存
    // @param memory: 计算参数
    __declspec(dllexport) void
    FreeResponseSpectrumConfig(ResponseSpectrumConfig *memory);

    // 获取FFT计算参数
    // @param config_file: 计算参数文件
    // @return FourierSpectrumConfig: 计算参数
    __declspec(dllexport) FourierSpectrumConfig *
    GetFourierSpectrumConfig(const char *config_file);

    // 释放FFT计算参数内存
    // @param memory: 计算参数
    __declspec(dllexport) void
    FreeFourierSpectrumConfig(FourierSpectrumConfig *memory);

#ifdef __cplusplus
}
#endif
#endif // GMP_LIBRARY_H_