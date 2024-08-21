/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\gmp_plot\gmp_plot.cpp
** -----
** File Created: Tuesday, 13th August 2024 11:11:31
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Tuesday, 13th August 2024 11:13:11
** Modified By: Dong Feiyue (donfeiyue@outlook.com>)
*/

// Description:
// 地震动参数计算结果绘制(Extern "C")的实现

// associated header
#include "gmp_plot.h"

// stdc++ headers
#include <vector>

// projects headers
#include "data_visualization/plotting_xy.h"
#include "gmp_calculation/gmp_calculation.h"


// 计算反应谱并绘制图形
void PlotResponseSpectrum(const double *acceleration,
                          int size,
                          double frequency,
                          double damping_ratio)
{
    // 创建计算对象
    std::vector<double> acc(acceleration, acceleration + size);
    gmp_calculation::GmpCalculation gmp(acc, frequency, damping_ratio);
    auto response = gmp.ResponseSpectrum();

    // 创建周期序列
    double dt = 0.01;
    std::vector<double> period;
    for (size_t i = 0; i < response.Sa.size(); i++)
    {
        period.push_back(i * dt);
    }

    // 绘制反应谱图
    data_visualization::PlotXY plot1(period,
                                     response.Sa,
                                     "Acceleration Response Spectrum",
                                     "Period (s)",
                                     "Sa (cm/s^2)");
    plot1.set_size(1600, 1200);
    plot1.set_origin(0, 0);
    plot1.DrawFile("output/Sa");

    data_visualization::PlotXY plot2(period,
                                     response.Sv,
                                     "Velocity Response Spectrum",
                                     "Period (s)",
                                     "Sv (cm/s)");
    plot2.set_size(1600, 1200);
    plot2.set_origin(0, 0);
    plot2.DrawFile("output/Sv");

    data_visualization::PlotXY plot3(period,
                                     response.Sd,
                                     "Displacement Response Spectrum",
                                     "Period (s)",
                                     "Sd (cm)");
    plot3.set_size(1600, 1200);
    plot3.set_origin(0, 0);
    plot3.DrawFile("output/Sd");
}

// 计算拟反应谱并绘制图形
void PlotPseudoResponseSpectrum(const double *acceleration,
                                int size,
                                double frequency,
                                double damping_ratio)
{
    // 创建计算对象
    std::vector<double> acc(acceleration, acceleration + size);
    gmp_calculation::GmpCalculation gmp(acc, frequency, damping_ratio);
    auto response = gmp.PseudoResponseSpectrum();

    // 创建周期序列
    double dt = 0.01;
    std::vector<double> period;
    for (size_t i = 1; i <= response.Sa.size(); i++)
    {
        period.push_back(i * dt);
    }

    // 绘制拟反应谱图
    data_visualization::PlotXY plot1(period,
                                     response.Sa,
                                     "Acceleration Pseudo Response Spectrum",
                                     "Period (s)",
                                     "Sa (cm/s^2)");
    plot1.set_size(1600, 1200);
    plot1.set_zoom({0, 0, 1.1, 1.1});
    plot1.set_origin(0, 0);
    plot1.DrawFile("output/Pseudo_Sa");

    data_visualization::PlotXY plot2(period,
                                     response.Sv,
                                     "Velocity Pseudo Response Spectrum",
                                     "Period (s)",
                                     "Sv (cm/s)");
    plot2.set_size(1600, 1200);
    plot2.set_zoom({0, 0, 1.1, 1.1});
    plot2.set_origin(0, 0);
    plot2.DrawFile("output/Pseudo_Sv");

    data_visualization::PlotXY plot3(period,
                                     response.Sd,
                                     "Displacement Pseudo Response Spectrum",
                                     "Period (s)",
                                     "Sd (cm)");
    plot3.set_size(1600, 1200);
    plot3.set_zoom({0, 0, 1.1, 1.1});
    plot3.set_origin(0, 0);
    plot3.DrawFile("output/Pseudo_Sd");
}

// 计算Fourier幅值谱并绘制图形
void PlotFourierSpectrum(const double *acceleration, int size, double frequency)
{
    // 创建计算对象
    std::vector<double> acc(acceleration, acceleration + size);
    gmp_calculation::GmpCalculation gmp(acc, frequency, 0.2);
    auto fourier_amp = gmp.FourierAmplitudeSpectrum();

    // 创建频率序列
    double df = frequency / size;
    std::vector<double> f;
    for (size_t i = 0; i < fourier_amp.size(); i++)
    {
        f.push_back(i * df);
    }

    // 绘制Fourier幅值谱图
    data_visualization::PlotXY plot1(f,
                                     fourier_amp,
                                     "Fourier Amplitude Spectrum",
                                     "Frequency (Hz)",
                                     "Amplitude");
    plot1.set_size(1600, 1200);
    plot1.set_origin(0, 0);
    plot1.set_ranges(
        {0,
         frequency / 5,
         0,
         *std::max_element(fourier_amp.begin(), fourier_amp.end())});
    plot1.DrawFile("output/Fourier_Amplitude");
}