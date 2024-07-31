#include <data_structure/acceleration.h>
#include "data_structure/building.h"
#include "test_function.h"

#include <fstream>
#include <iosfwd>
#include <iostream>
#include <string>
#include <vector>


using namespace std;

void test_modified_filter_integrate()
{
    // 读取数据文件
    string file_name = "acceleration_data/test_acceleration.txt";

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
    acceleration.data() = readMatrixFromFile(file_name);
    edp_calculation::ModifiedFilteringIntegral m_filt_integral(
        acceleration, building, 2);
    m_filt_integral.CalculateEdp();
    auto result = m_filt_integral.get_filtering_interp_result();
    auto drift = result.get_story_drift();
    auto displacement = result.get_displacement();

    ofstream ofs1("acceleration_data/test_story_drift.txt");
    ofstream ofs2("acceleration_data/test_displacement.txt");
    // 输出结果
    for (int i = 0; i < drift.data().front().size(); ++i)
    {
        for (int j = 0; j < drift.data().size(); ++j)
        {
            ofs1 << drift.data().at(j)[i] << " ";
            ofs2 << displacement.data().at(j)[i] << " ";
        }
        ofs1 << endl;
        ofs2 << endl;
    }
    ofs1.close();
    ofs2.close();
}