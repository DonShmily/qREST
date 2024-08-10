/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\data_structure\basic_data_structure.cpp
** -----
** File Created: Thursday, 4th July 2024 22:56:12
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Saturday, 10th August 2024 22:24:54
** Modified By: Dong Feiyue (donfeiyue@outlook.com>)
*/

// Description:
// 数据结构基本类的实现。

// associated header
#include "basic_data_structure.h"

// stdc++ headers
#include <iosfwd>
#include <iostream>
#include <stdexcept>

// project headers
#include "numerical_algorithm/vector_calculation.h"


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

// 计算每一列信号的能量
std::vector<double> BasicData::Energy() const
{
    std::vector<double> energy(0);
    for (const auto &col : *data_)
    {
        energy.push_back(
            numerical_algorithm::CrossCorrelationAtShift(col, col, 0));
    }
    return energy;
}

// 计算每一列信号的正向最大值
std::vector<std::pair<double, size_t>> BasicData::PositiveMax() const
{
    std::vector<std::pair<double, size_t>> max(0);
    for (const auto &col : *data_)
    {
        auto max_index = std::max_element(col.begin(), col.end());
        max.push_back(std::make_pair(*max_index, max_index - col.begin()));
    }
    return max;
}

// 计算每一列信号的负向最大值
std::vector<std::pair<double, size_t>> BasicData::NegativeMax() const
{
    std::vector<std::pair<double, size_t>> max(0);
    for (const auto &col : *data_)
    {
        auto max_index = std::min_element(col.begin(), col.end());
        max.push_back(std::make_pair(*max_index, max_index - col.begin()));
    }
    return max;
}

// 计算每一列信号的绝对最大值
std::vector<std::pair<double, size_t>> BasicData::AbsoluteMax() const
{
    std::vector<std::pair<double, size_t>> max(0);
    for (const auto &col : *data_)
    {
        auto max_index = std::max_element(
            col.begin(), col.end(), [](const double &a, const double &b) {
                return std::abs(a) < std::abs(b);
            });
        max.push_back(std::make_pair(*max_index, max_index - col.begin()));
    }
    return max;
}

} // namespace data_structure
