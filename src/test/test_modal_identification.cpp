#include "test_function.h"

#include <fstream>
#include <iostream>
#include <string>

#include "parameters_identification/simple_parameters_identification.h"

using namespace std;

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

void test_modal_identification()
{
    std::cout << "Test modal identification." << std::endl;
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
        readMatrixFromFile(file_name);
    data_structure::Acceleration acceleration(test_acceleration, 50);
    parameters_identification::SimpleParametersIdentification spi(acceleration,
                                                                  building);
    spi.Identify();

    // 输出结果
    std::cout << "Frequency: ";
    for (auto &freq : spi.identify_parameters().get_frequency())
    {
        std::cout << freq << " ";
    }
    std::cout << std::endl;

    std::cout << "Damping ratio: ";
    for (auto &damping_ratio : spi.identify_parameters().get_damping_ratio())
    {
        std::cout << damping_ratio << " ";
    }
    std::cout << std::endl;

    std::cout << "Mode shape: " << std::endl;
    for (auto &mode : spi.identify_parameters().get_mode_shape())
    {
        for (auto &val : mode)
        {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "End test modal identification." << std::endl;
}