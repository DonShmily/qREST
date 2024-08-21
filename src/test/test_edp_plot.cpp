#include "edp_plot/edp_plot.h"
#include "test_function.h"

#include <fstream>
#include <vector>

// 测试EDP绘图
void test_edp_plot()
{
    // 读取文件中的数据
    std::vector<double> floor, measurement;
    std::ifstream ifs("building/floor.txt");
    double temp;
    while (ifs >> temp)
    {
        floor.push_back(temp);
    }
    ifs.close();
    ifs.open("building/measurement.txt");
    while (ifs >> temp)
    {
        measurement.push_back(temp);
    }
    ifs.close();

    // 创建计算对象
    auto acceleration = ReadMatrixFromFile("acceleration_data/accNS.txt");

    // 计算对象赋值
    Building_C *building_c = new Building_C;
    *building_c = {
        floor.data(), floor.size(), measurement.data(), measurement.size()};
    size_t col_number = acceleration.size(),
           row_number = acceleration.front().size();
    double *acc_c = new double[col_number * row_number];
    for (std::size_t i = 0; i < col_number; ++i)
    {
        std::copy(acceleration.at(i).begin(),
                  acceleration.at(i).end(),
                  acc_c + i * row_number);
    }

    // 绘制图形
    PlotMaxIdr(acc_c, row_number, 50, building_c);
}