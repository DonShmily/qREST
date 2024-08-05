#include "header.inc"

#include "test_function.h"

#include <fstream>
#include <iostream>
#include <string>

void test_vector_operation()
{
    // 读取数据文件
    std::string file_name = "vector_operation/test_vector1.txt";
    std::vector<double> vec1, vec2;
    std::ifstream ifs(file_name);
    double temp;
    while (ifs >> temp)
    {
        vec1.push_back(temp);
    }
    ifs.close();
    file_name = "vector_operation/test_vector2.txt";
    ifs.open(file_name);
    while (ifs >> temp)
    {
        vec2.push_back(temp);
    }
    ifs.close();

    // 向量计算
    auto result = numerical_algorithm::VectorOperation(vec1, vec2, '+');

    // 输出结果
    std::ofstream ofs("vector_operation/test_vector_result.txt");
    for (auto &val : result)
    {
        ofs << val << " ";
    }
    ofs.close();
}