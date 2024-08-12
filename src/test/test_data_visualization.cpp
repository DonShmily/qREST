#include "data_structure/acceleration.h"
#include "data_structure/building.h"
#include "data_visualization/plotting_xy.h"
#include "data_visualization/plotting_xy_multi.h"
#include "test_function.h"


#include <fstream>
#include <iosfwd>
#include <iostream>
#include <string>
#include <vector>


using namespace std;

void test_data_visualization()
{
    // 读取数据文件
    string file_name = "acceleration_data/accNS.txt";

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
    auto building = data_structure::Building(measurement, floor);
    auto acceleration = data_structure::Acceleration(
        std::vector<std::vector<double>>(), 50, 0.01);
    acceleration.data() = ReadMatrixFromFile(file_name);

    edp_calculation::ModifiedFilteringIntegral m_filt_integral(
        acceleration, building, 2);
    m_filt_integral.CalculateEdp();

    auto result = m_filt_integral.get_filtering_interp_result();
    auto drift = result.get_inter_story_drift();
    auto displacement = result.get_displacement();

    // 绘制结果
    std::vector<double> time(displacement.get_row_number());
    double dt = displacement.get_time_step();
    for (size_t i = 0; i < time.size(); ++i)
    {
        time[i] = i * dt;
    }
    data_visualization::PlotXY plot(time,
                                    displacement.get_col(0),
                                    "Displacement",
                                    "Time (s)",
                                    "Displacement (cm)");
    plot.DrawFile("output/picture/test");
}