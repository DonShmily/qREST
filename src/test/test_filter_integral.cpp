#include <fstream>
#include <iosfwd>
#include <iostream>
#include <string>
#include <vector>

#include "data_structure/acceleration.h"
#include "data_structure/building.h"
#include "edp_calculation/filtering_integral.h"
#include "numerical_algorithm/basic_filtering.h"


using namespace std;

static std::vector<std::vector<double>>
readMatrixFromFile1(const string &filename)
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

void test_filter_integrate()
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
    data_structure::Building building(measurement, floor);
    std::vector<std::vector<double>> test_acceleration =
        readMatrixFromFile1(file_name);
    data_structure::Acceleration acceleration(test_acceleration, 50);
    edp_calculation::FilteringIntegral filtering_integral(
        acceleration, building, 2, 0.1, 20);
    filtering_integral.get_filtering_interp_method().filter_function_ =
        numerical_algorithm::FilterFunction::filter;

    filtering_integral.CalculateEdp();
    auto result = filtering_integral.get_filtering_interp_result();
    auto drift = result.get_inter_story_drift();
    auto displacement = result.get_displacement();

    ofstream ofs1("test_acceleration/test_story_drift.txt");
    ofstream ofs2("test_acceleration/test_displacement.txt");
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