#include "test_function.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;
using namespace Eigen;

std::vector<std::vector<double>> readMatrixFromFile(const string &filename)
{
    ifstream file(filename);
    string line;
    vector<vector<double>> data;

    // Read the file line by line
    while (getline(file, line))
    {
        stringstream lineStream(line);
        vector<double> row;
        double value;

        // Parse each line into a row of values
        while (lineStream >> value)
        {
            row.push_back(value);
        }
        data.push_back(row);
    }

    std::vector<std::vector<double>> data_transpose(
        data.front().size(), std::vector<double>(data.size()));
    for (std::size_t i = 0; i != data.size(); ++i)
        for (std::size_t j = 0; j != data.front().size(); ++j)
            data_transpose[j][i] = data[i][j];
    return data_transpose;
}

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
    data_structure::Building building(measurement, floor);
    std::vector<std::vector<double>> test_acceleration =
        readMatrixFromFile(file_name);
    data_structure::Acceleration acceleration(test_acceleration, 50, 0.01);
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
            ofs1 << drift.data()[j][i] << " ";
            ofs2 << displacement.data()[j][i] << " ";
        }
        ofs1 << endl;
        ofs2 << endl;
    }
    ofs1.close();
    ofs2.close();
}