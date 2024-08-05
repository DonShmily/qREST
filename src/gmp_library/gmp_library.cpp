// associated header
#include "gmp_library.h"

// stdc++ headers
#include <algorithm>
#include <vector>

// projects headers
#include "gmp_calculation/gmp_calculation.h"

response_spectrum *ResponseSpectrum(double frequency,
                                    double damping_ratio,
                                    const double *acceleration,
                                    int size)
{
    // 创建计算对象
    std::vector<double> acc(acceleration, acceleration + size);
    gmp_calculation::GmpCalculation gmp(acc, frequency, damping_ratio);
    auto response = gmp.get_response_spectrum();

    // 创建输出对象
    response_spectrum *result = new response_spectrum;

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

response_spectrum *PseudoResponseSpectrum(double frequency,
                                          double damping_ratio,
                                          const double *acceleration,
                                          int size)
{
    // 创建计算对象
    std::vector<double> acc(acceleration, acceleration + size);
    gmp_calculation::GmpCalculation gmp(acc, frequency, damping_ratio);
    auto response = gmp.get_pseudo_response_spectrum();

    // 创建输出对象
    response_spectrum *result = new response_spectrum;

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

void FreeMemory(response_spectrum *memory)
{
    delete[] memory->Sa;
    delete[] memory->Sv;
    delete[] memory->Sd;
    delete memory;
}

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