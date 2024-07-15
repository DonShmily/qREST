#include "test_function.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "eigen3/Eigen/Dense"

using namespace std;
using namespace Eigen;

MatrixXd readMatrixFromFile(const string &filename)
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

    // Convert the vector of vectors to an Eigen::MatrixXd
    int rows = data.size();
    int cols = rows > 0 ? data[0].size() : 0;
    MatrixXd matrix(rows, cols);

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            matrix(i, j) = data[i][j];
        }
    }

    return matrix;
}

void test_filter_integrate()
{
    string file_name = "test_acceleration/test_acceleration.txt";
    MatrixXd test_acceleration = readMatrixFromFile(file_name);

    std::vector<double> floor, measurement;
    std::ifstream ifs("building/floor.txt");
    while (!ifs.eof())
    {
        double temp;
        ifs >> temp;
        floor.push_back(temp);
    }
    ifs.open("building/measurement");
    while (!ifs.eof())
    {
        double temp;
        ifs >> temp;
        measurement.push_back(temp);
    }
    ifs.close();
}