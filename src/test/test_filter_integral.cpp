// #include "test_function.h"
//
// #include <fstream>
// #include <iostream>
// #include <sstream>
// #include <vector>
//
// #include "eigen3/Eigen/Dense"
//
// using namespace std;
// using namespace Eigen;
//
// MatrixXd readMatrixFromFile(const string &filename)
//{
//     ifstream file(filename);
//     string line;
//     vector<vector<double>> data;
//
//     // Read the file line by line
//     while (getline(file, line))
//     {
//         stringstream lineStream(line);
//         vector<double> row;
//         double value;
//
//         // Parse each line into a row of values
//         while (lineStream >> value)
//         {
//             row.push_back(value);
//         }
//         data.push_back(row);
//     }
//
//     // Convert the vector of vectors to an Eigen::MatrixXd
//     int rows = data.size();
//     int cols = rows > 0 ? data[0].size() : 0;
//     MatrixXd matrix(rows, cols);
//
//     for (int i = 0; i < rows; ++i)
//     {
//         for (int j = 0; j < cols; ++j)
//         {
//             matrix(i, j) = data[i][j];
//         }
//     }
//
//     return matrix;
// }
//
// void test_filter_integrate()
//{
//     // 读取数据文件
//     string file_name = "acceleration_data/test_acceleration.txt";
//
//     std::vector<double> floor, measurement;
//     std::ifstream ifs("building/floor.txt");
//     while (!ifs.eof())
//     {
//         double temp;
//         ifs >> temp;
//         floor.push_back(temp);
//     }
//     ifs.open("building/measurement");
//     while (!ifs.eof())
//     {
//         double temp;
//         ifs >> temp;
//         measurement.push_back(temp);
//     }
//     ifs.close();
//
//     // 创建计算对象
//     data_structure::Building building(measurement, floor);
//     MatrixXd test_acceleration = readMatrixFromFile(file_name);
//     data_structure::Acceleration acceleration(test_acceleration, 50);
//     edp_calculation::ModifiedFilteringIntegral m_filt_integral(
//         acceleration, building, 2);
//     m_filt_integral.CalculateEdp();
//     auto result = m_filt_integral.get_filtering_interp_result();
//     auto drift = result.get_story_drift();
//     auto displacement = result.get_displacement();
//
//     ofstream ofs1("test_acceleration/test_story_drift.txt");
//     ofstream ofs2("test_acceleration/test_displacement.txt");
//     // 输出结果
//     for (int i = 0; i < drift.data().rows(); ++i)
//     {
//         for (int j = 0; j < drift.data().cols(); ++j)
//         {
//             ofs1 << drift.data()(i, j) << " ";
//             ofs2 << displacement.data()(i, j) << " ";
//         }
//         ofs1 << endl;
//         ofs2 << endl;
//     }
//     ofs1.close();
//     ofs2.close();
// }