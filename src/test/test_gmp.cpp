#include <fstream>
#include <iosfwd>
#include <ostream>
#include <string>
#include <vector>

#include "gmp_calculation/gmp_calculation.h"
#include "test_function.h"


using namespace std;
using namespace gmp_calculation;

void test_gmp()
{
    // 读取数据文件
    string file_name = "acceleration_data/accNS.txt";
    // 创建计算对象
    std::vector<std::vector<double>> test_acceleration =
        ReadMatrixFromFile(file_name);
    auto &acc = test_acceleration[0];
    GmpCalculation gmp(acc, 50, 0.2);

    // 创建输出对象
    std::ofstream spectrum_out("acceleration_data/spectrum_res_.txt");
    spectrum_out << "******************** Ti = 2s ********************"
                 << std::endl;

    auto spectrum_t2_result = gmp.ResponseSpectrumTi(2);
    auto ps_spectrum_t2_result = gmp.PseudoResponseSpectrumTi(2);

    spectrum_out << spectrum_t2_result.SaTi << " " << spectrum_t2_result.SvTi
                 << " " << spectrum_t2_result.SdTi << " "
                 << ps_spectrum_t2_result.SaTi << " "
                 << ps_spectrum_t2_result.SvTi << " "
                 << ps_spectrum_t2_result.SdTi << std::endl;
    spectrum_out.close();

    spectrum_out.open("acceleration_data/spectrum_res.txt");
    auto spectrum_result = gmp.ResponseSpectrum();
    auto ps_spectrum_result = gmp.PseudoResponseSpectrum();

    for (int i = 0; i < spectrum_result.Sa.size(); i++)
    {
        spectrum_out << spectrum_result.Sa[i] << " " << spectrum_result.Sv[i]
                     << " " << spectrum_result.Sd[i] << " "
                     << ps_spectrum_result.Sa[i] << " "
                     << ps_spectrum_result.Sv[i] << " "
                     << ps_spectrum_result.Sd[i] << "\n";
    }
    spectrum_out.close();

    spectrum_out.open("acceleration_data/fourier_res.txt");
    spectrum_out << "******************** Fourier ********************"
                 << std::endl;
    auto fourier_amp_result = gmp.FourierAmplitudeSpectrum();
    auto fourier_pow_result = gmp.PowerSpectrum();

    for (int i = 0; i < fourier_amp_result.size(); i++)
    {
        spectrum_out << fourier_amp_result[i] << " " << fourier_pow_result[i]
                     << "\n";
    }
    spectrum_out.close();
}