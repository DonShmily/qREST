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
#include <iosfwd>
#include <iostream>
#include <stdexcept>


namespace data_structure
{

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
    resize(row_number, col_number);
    for (std::size_t i = 0; i < row_number; ++i)
    {
        for (std::size_t j = 0; j < col_number; ++j)
        {
            stream >> data_->at(j).at(i);
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
    resize(row_number, col_number, init_value);
}

// 改变数据矩阵的大小
void BasicData::resize(const std::size_t &row_number,
                       const std::size_t &col_number,
                       const double &init_value)
{
    data_->resize(col_number);
    for (std::size_t i = 0; i < col_number; ++i)
    {
        data_->at(i).resize(row_number, init_value);
    }
}

} // namespace data_structure
