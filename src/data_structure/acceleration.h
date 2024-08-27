/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\data_structure\acceleration.h
** -----
** File Created: Saturday, 6th July 2024 15:15:09
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Saturday, 10th August 2024 22:24:49
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 加速度信息类，数据基类的派生，存储加速度数据的基本信息。
// 数据上增加了加速度采样频率的信息。
// 功能上在构造加速度信息时增加了调整加速度均值为0的功能；增加了从指定列构造加速度信息的功能。

#ifndef DATA_STRUCTURE_ACCELERATION_H
#define DATA_STRUCTURE_ACCELERATION_H


// stdc++ headers
#include <algorithm>
#include <initializer_list>
#include <iosfwd>
#include <numeric>
#include <vector>

// project headers
#include "basic_data_structure.h"


namespace data_structure
{
// 加速度信息类
class Acceleration : public BasicData
{
public:
    // 默认构造函数
    Acceleration() : BasicData() {}

    // 从二维std::vector<std::vector<double>>构造，每个vector<double>代表一个测点的加速度数据
    // @param matrix 加速度数据矩阵
    // @param frequency 采样频率
    // @param scale 调幅因子
    Acceleration(const std::vector<std::vector<double>> &matrix,
                 const double &frequency,
                 const double &scale = 1.0)
        : BasicData(matrix), frequency_(frequency)
    {
        adjust_acceleration(scale);
    }

    // 从std::shared_ptr<std::vector<std::vector<double>>>构造，每个vector<double>代表一个测点的加速度数据
    // @param matrix_ptr 加速度数据矩阵指针
    // @param frequency 采样频率
    // @param scale 调幅因子
    Acceleration(
        const std::shared_ptr<std::vector<std::vector<double>>> &matrix_ptr,
        const double &frequency,
        const double &scale = 1.0)
        : BasicData(matrix_ptr), frequency_(frequency)
    {
        adjust_acceleration(scale);
    }

    // 从输入流构造，col_number为测点数量，row_number为每个测点的数据长度
    // @param stream 输入流
    // @param row_number 每个测点的数据长度
    // @param col_number 测点数量
    // @param frequency 采样频率
    // @param scale 调幅因子
    Acceleration(std::istream &stream,
                 std::size_t row_number,
                 std::size_t col_number,
                 const double &frequency,
                 const double &scale = 1.0)
        : BasicData(stream, row_number, col_number), frequency_(frequency)
    {
        adjust_acceleration(scale);
    }

    // 从指定列构造，从整体的加速度类型变量的部分列构造，col_index为指定列的索引
    // @param acceleration 加速度信息
    // @param frequency 采样频率
    // @param col_index 指定列的索引
    // @param scale 调幅因子
    Acceleration(const Acceleration &acceleration,
                 const std::vector<std::size_t> &col_index,
                 const double &frequency,
                 const double &scale = 1.0);

    // 从指定列的std::vector构造，从整体的加速度类型变量的部分列构造，col_index为指定列的索引
    // @param acceleration_vector 加速度数据
    // @param frequency 采样频率
    // @param col_index 指定列的索引
    // @param scale 调幅因子
    Acceleration(const std::vector<std::vector<double>> &acceleration_vector,
                 const std::vector<std::size_t> &col_index,
                 const double &frequency,
                 const double &scale = 1.0);

    // 析构函数
    ~Acceleration() = default;

    // 获取采样频率
    double get_frequency() const { return frequency_; }

    // 获取时间步长
    double get_time_step() const { return 1.0 / frequency_; }

    // 设置采样频率
    // @param frequency 新的采样频率
    void set_frequency(const double &frequency) { frequency_ = frequency; }

    // 求解相对加速度（相对于底层），返回2-top测点的相对加速度信息
    Acceleration relative_acceleration() const;

    // 求解层间相对加速度，返回2-top相邻层间的相对加速度信息
    Acceleration interstory_acceleration() const;

private:
    // 成员变量：采样频率
    double frequency_{};

    // 调整输入加速度使其各列均值为0，并作调幅处理
    inline void adjust_acceleration(const double &scale)
    {
        for (std::size_t i = 0; i < data_->size(); ++i)
        {
            const double col_mean =
                std::accumulate(data_->at(i).begin(), data_->at(i).end(), 0.0)
                / data_->at(i).size();
            // 如果事先介入了数据异常处理模块，则一般可以保证所有信号的均值为0
            // const double col_mean = 0;
            std::transform(data_->at(i).begin(),
                           data_->at(i).end(),
                           data_->at(i).begin(),
                           [col_mean, scale](const double &val) {
                               return (val - col_mean) * scale;
                           });
        }
    }

    // 从指定大小构造
    Acceleration(const std::size_t &row_number,
                 const std::size_t &col_number,
                 const double &frequency,
                 const double &init_value = 0.0)
        : BasicData(row_number, col_number, init_value), frequency_(frequency)
    {}
};

} // namespace data_structure

#endif // DATA_STRUCTURE_ACCELERATION_H
