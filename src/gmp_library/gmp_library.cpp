/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\gmp_library\gmp_library.cpp
** -----
** File Created: Monday, 5th August 2024 17:40:31
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Saturday, 10th August 2024 22:27:22
** Modified By: Dong Feiyue (donfeiyue@outlook.com>)
*/

// Description:
// 地震动参数计算库(Extern "C")的实现

// associated header
#include "gmp_library.h"

// stdc++ headers
#include <algorithm>
#include <vector>

// projects headers
#include "gmp_calculation/gmp_calculation.h"

// 计算反应谱
ResponseSpectrum *GetResponseSpectrum(const double *acceleration,
                                      int size,
                                      double frequency,
                                      double damping_ratio)
{
    // 创建计算对象
    std::vector<double> acc(acceleration, acceleration + size);
    gmp_calculation::GmpCalculation gmp(acc, frequency, damping_ratio);
    auto response = gmp.get_response_spectrum();

    // 创建输出对象
    ResponseSpectrum *result = new ResponseSpectrum;

    result->Sa = new double[500];
    result->Sv = new double[500];
    result->Sd = new double[500];
    std::copy(response.Sa.begin(), response.Sa.end(), result->Sa);
    std::copy(response.Sv.begin(), response.Sv.end(), result->Sv);
    std::copy(response.Sd.begin(), response.Sd.end(), result->Sd);

    result->result_size = 500;
    result->dt = 0.01;

    return result;
}

// 计算拟反应谱
ResponseSpectrum *GetPseudoResponseSpectrum(const double *acceleration,
                                            int size,
                                            double frequency,
                                            double damping_ratio)
{
    // 创建计算对象
    std::vector<double> acc(acceleration, acceleration + size);
    gmp_calculation::GmpCalculation gmp(acc, frequency, damping_ratio);
    auto response = gmp.get_pseudo_response_spectrum();

    // 创建输出对象
    ResponseSpectrum *result = new ResponseSpectrum;

    result->Sa = new double[500];
    result->Sv = new double[500];
    result->Sd = new double[500];
    std::copy(response.Sa.begin(), response.Sa.end(), result->Sa);
    std::copy(response.Sv.begin(), response.Sv.end(), result->Sv);
    std::copy(response.Sd.begin(), response.Sd.end(), result->Sd);

    result->result_size = 500;
    result->dt = 0.01;

    return result;
}

// 释放反应谱结果内存
void FreeResponseSpectrum(ResponseSpectrum *memory)
{
    if (memory != NULL)
    {
        delete[] memory->Sa;
        delete[] memory->Sv;
        delete[] memory->Sd;
        delete memory;
    }
}

// 计算Fourier幅值谱
double *FourierSpectrum(const double *acceleration, int size)
{
    // 创建计算对象
    std::vector<double> acc(acceleration, acceleration + size);
    gmp_calculation::GmpCalculation gmp(acc, 0, 0);
    auto fourier_result = gmp.FourierAmplitudeSpectrum();

    // 创建输出对象
    double *result = new double[size];
    std::copy(fourier_result.begin(), fourier_result.end(), result);

    return result;
}

// 释放double数组内存
void FreeArray(double *memory)
{
    if (memory != NULL)
    {
        delete[] memory;
    }
}