#include "header.inc"

#include "test_function.h"

#include <vector>

// 测试GMP绘图
void test_gmp_plot()
{
    // 创建计算对象
    std::vector<std::vector<double>> test_acceleration =
        ReadMatrixFromFile("acceleration_data/accNS.txt");
    auto &acc = test_acceleration[0];

    // 绘制图形
    PlotResponseSpectrum(acc.data(), acc.size(), 50, 0.05);
    PlotPseudoResponseSpectrum(acc.data(), acc.size(), 50, 0.05);
    PlotFourierSpectrum(acc.data(), acc.size(), 50);
}