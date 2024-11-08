﻿/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\data_structure\displacement.h
** -----
** File Created: Tuesday, 9th July 2024 14:14:23
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Saturday, 10th August 2024 22:25:19
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 位移信息类，数据基类的派生，存储位移数据的基本信息。

#ifndef DATA_STRUCTURE_DISPLACEMENT_H_
#define DATA_STRUCTURE_DISPLACEMENT_H_

// stdc++ headers
#include <iosfwd>
#include <vector>

// project headers
#include "basic_data_structure.h"

namespace data_structure
{
// 位移信息类
class Displacement : public BasicData
{
public:
    // 默认构造函数
    Displacement() : BasicData() {}

    // 从二维std::vector<std::vector<double>>构造，每个vector<double>代表一个测点的位移数据
    // @param matrix 位移数据矩阵
    // @param frequency 采样频率
    Displacement(const std::vector<std::vector<double>> &matrix,
                 const double &frequency)
        : BasicData(matrix), frequency_(frequency)
    {}

    // 从std::shared_ptr<std::vector<std::vector<double>>>构造，每个vector<double>代表一个测点的位移数据
    // @param matrix_ptr 位移数据矩阵指针
    // @param frequency 采样频率
    Displacement(
        const std::shared_ptr<std::vector<std::vector<double>>> &matrix_ptr,
        const double &frequency)
        : BasicData(matrix_ptr), frequency_(frequency)
    {}

    // 从输入流构造，col_number为测点数量，row_number为每个测点的数据长度
    // @param stream 输入流
    // @param row_number 每个测点的数据长度
    // @param col_number 测点数量
    // @param frequency 采样频率
    Displacement(std::istream &stream,
                 std::size_t row_number,
                 std::size_t col_number,
                 const double &frequency)
        : BasicData(stream, row_number, col_number), frequency_(frequency)
    {}

    // 析构函数
    ~Displacement() = default;

    // 获取位移采样频率
    double get_frequency() const { return frequency_; }

    // 获取位移时间步长
    double get_time_step() const { return 1.0 / frequency_; }

    // 设置采样频率
    // @param frequency 新的采样频率
    void set_frequency(const double &frequency) { frequency_ = frequency; }

    // 求解相对位移（相对于底层），返回2-top测点的相对位移信息
    Displacement relative_displacement() const;

    // 求解层间相对位移，返回2-top相邻层间的相对位移信息
    Displacement interstory_displacement() const;

private:
    // 位移采样频率
    double frequency_{};

    // 从指定大小构造
    // @param row_number 行数
    // @param col_number 列数
    // @param frequency 采样频率
    // @param init_value 初始化值
    Displacement(const std::size_t &row_number,
                 const std::size_t &col_number,
                 const double frequency,
                 const double &init_value = 0.0)
        : BasicData(row_number, col_number, init_value), frequency_(frequency)
    {}
};

} // namespace data_structure

#endif // DATA_STRUCTURE_DISPLACEMENT_H_