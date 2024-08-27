/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\gmp_plot\gmp_plot.h
** -----
** File Created: Tuesday, 13th August 2024 11:03:15
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Tuesday, 13th August 2024 11:03:26
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 地震动参数计算结果绘制(Extern "C")

#ifndef GMP_PLOT_H_
#define GMP_PLOT_H_

#ifdef __cplusplus
extern "C"
{
#endif

    // 计算反应谱并绘制图形
    // @param acceleration: 加速度数据
    // @param size: 加速度数据长度
    // @param frequency: 采样频率
    // @param damping_ratio: 阻尼比
    __declspec(dllexport) void PlotResponseSpectrum(const double *acceleration,
                                                    int size,
                                                    double frequency,
                                                    double damping_ratio);

    // 计算拟反应谱并绘制图形
    // @param acceleration: 加速度数据
    // @param size: 加速度数据长度
    // @param frequency: 采样频率
    // @param damping_ratio: 阻尼比
    __declspec(dllexport) void
    PlotPseudoResponseSpectrum(const double *acceleration,
                               int size,
                               double frequency,
                               double damping_ratio);

    // 计算Fourier幅值谱并绘制图形
    // @param acceleration: 加速度数据
    // @param size: 加速度数据长度
    __declspec(dllexport) void
    PlotFourierSpectrum(const double *acceleration, int size, double frequency);

#ifdef __cplusplus
}
#endif
#endif // GMP_PLOT_H_