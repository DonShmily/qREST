#include "test_function.h"

#include "edp_library/edp_library.h"

#include <cstddef>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <string>
#include <vector>


using namespace std;

void test_edp_library()
{
    // 读取数据文件
    string file_name = "acceleration_data/accEW.txt";

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

    // 计算对象赋值
    auto floor_height = building.get_floor_height();
    auto measure_height = building.get_measuren_height();
    ::building *building_c = new ::building;
    *building_c = {floor_height.data(),
                   floor_height.size(),
                   measure_height.data(),
                   measure_height.size()};
    size_t col_number = acceleration.get_col_number();
    double **acc_c = new double *[col_number];
    for (std::size_t i = 0; i < col_number; ++i)
    {
        acc_c[i] = new double[acceleration.get_row_number()];
        std::copy(
            acceleration.col(i).begin(), acceleration.col(i).end(), acc_c[i]);
    }

    auto idr_result = ModifiedFilteringIntegral(
        acc_c, acceleration.get_row_number(), 50, building_c);

    std::ofstream ofs1("acceleration_data/C2_test_story_drift.txt");
    for (std::size_t i = 0; i < idr_result->time_step_count; ++i)
    {
        for (std::size_t j = 0; j < idr_result->story_count; ++j)
        {
            ofs1 << idr_result->inter_story_drift[j][i] << " ";
        }
        ofs1 << endl;
    }
}