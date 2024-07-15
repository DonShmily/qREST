/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/data_structure/basic_data_structure.cpp
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
// Description:
// 数据结构基本类的实现。

// associated header
#include "basic_data_structure.h"

// stdc++ headers
#include <iostream>
#include <istream>
#include <stdexcept>
#include <vector>


namespace data_structure
{

// 从二维std::vector<std::vector<double>>构造
BasicData::BasicData(const std::vector<std::vector<double>> &matrix)
{
    data_.resize(matrix.size(), matrix[0].size());
    for (std::size_t i = 0; i < matrix.size(); ++i)
    {
        for (std::size_t j = 0; j < matrix[0].size(); ++j)
        {
            data_(i, j) = matrix[i][j];
        }
    }
}

// 从输入流构造
BasicData::BasicData(std::istream &stream,
                     std::size_t row_number,
                     std::size_t col_number)
{
    try
    {
        if (row_number == 0)
        {
            throw std::invalid_argument("Row number number is zero.");
        }
        if (col_number == 0)
        {
            throw std::invalid_argument("Column number number is zero.");
        }
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << e.what() << std::endl;
    }
    try
    {
        if (!stream)
        {
            throw std::invalid_argument("Input stream is invalid.");
        }
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << e.what() << std::endl;
    }
    data_.resize(row_number, col_number);
    for (std::size_t i = 0; i < row_number; ++i)
    {
        for (std::size_t j = 0; j < col_number; ++j)
        {
            stream >> data_(i, j);
        }
    }
}

BasicData::BasicData(const std::size_t &row_number,
                     const std::size_t &col_number,
                     const double &init_value)
{
    try
    {
        if (row_number == 0)
        {
            throw std::invalid_argument("Row number number is zero.");
        }
        if (col_number == 0)
        {
            throw std::invalid_argument("Column number number is zero.");
        }
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << e.what() << std::endl;
    }
    data_.resize(row_number, col_number);
    data_.fill(init_value);
}

} // namespace data_structure
