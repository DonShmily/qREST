/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/data_structure/velocity.h
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
// Description:
// 速度信息类，数据基类的派生，存储速度数据的基本信息。

#ifndef SRC_DATA_STRUCTURE_VELOCITY_H_
#define SRC_DATA_STRUCTURE_VELOCITY_H_

// stdc++ headers
#include <iostream>
#include <vector>

// third-party library headers
#include "eigen3/Eigen/Core"

// project headers
#include "basic_data_structure.h"

namespace data_structure
{
// 速度信息类
class Velocity : public BasicData
{
public:
    // 默认构造函数
    Velocity() = default;
    // 从二维std::vector<std::vector<double>>构造，每个vector<double>代表一个测点的速度数据
    // @param matrix 速度数据矩阵
    // @param frequency 采样频率
    Velocity(const std::vector<std::vector<double>> &matrix,
             const double &frequency)
        : BasicData(matrix), frequency_(frequency)
    {}
    // 从输入流构造，col_number为测点数量，row_number为每个测点的数据长度
    // @param stream 输入流
    // @param row_number 每个测点的数据长度
    // @param col_number 测点数量
    // @param frequency 采样频率
    Velocity(std::istream &stream,
             std::size_t row_number,
             std::size_t col_number,
             const double &frequency)
        : BasicData(stream, row_number, col_number), frequency_(frequency)
    {}
    // 从Eigen::MatrixXd构造，矩阵每列代表一个测点的速度数据
    // @param matrix 速度数据矩阵
    // @param frequency 采样频率
    Velocity(const Eigen::MatrixXd &matrix, const double &frequency)
        : BasicData(matrix), frequency_(frequency)
    {}
    // 拷贝构造函数
    Velocity(const Velocity &data) = default;
    // 移动构造函数
    Velocity(Velocity &&data) = default;
    // 析构函数
    ~Velocity() = default;

    // 获取速度采样频率
    double get_frequency() const { return frequency_; }
    // 获取速度时间步长
    double get_time_step() const { return 1.0 / frequency_; }

    // 求解相对速度（相对于底层），返回2-top测点的相对速度信息
    Velocity relative_acceleration() const;
    // 求解层间相对速度，返回2-top相邻层间的相对速度信息
    Velocity interstory_acceleration() const;

private:
    // 速度采样频率
    double frequency_;

    // 从指定大小构造
    // @param row_number 行数
    // @param col_number 列数
    // @param init_value 初始化值
    // @param frequency 采样频率
    Velocity(const std::size_t &row_number,
             const std::size_t &col_number,
             const double &frequency,
             const double &init_value = 0.0)
        : BasicData(row_number, col_number, init_value), frequency_(frequency)
    {}
};
} // namespace data_structure

#endif // SRC_DATA_STRUCTURE_VELOCITY_H_