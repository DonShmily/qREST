/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/data_structure/basic_data_structure.h
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
// Description:
// 数据结构基本类，存储各类计算数据地类，如加速度、位移、速度等。基类提供基础功能，如文件读写、数据访问等。

#ifndef DATA_STRUCTURE_BASIC_DATA_H
#define DATA_STRUCTURE_BASIC_DATA_H

// stdc++ headers
#include <cstddef>
#include <iosfwd>
#include <memory>
#include <utility>
#include <vector>

// project headers
#include "numerical_algorithm/vector_calculation.h"


namespace data_structure
{

class BasicData
{
public:
    // 默认构造函数
    BasicData() = default;

    // 从二维std::vector<std::vector<double>>构造
    // @param matrix 数据矩阵
    BasicData(const std::vector<std::vector<double>> &matrix)
        : data_(std::make_shared<std::vector<std::vector<double>>>(matrix))
    {}

    // 从std::shared_ptr<std::vector<std::vector<double>>>构造
    // @param matrix_ptr 数据矩阵
    BasicData(
        const std::shared_ptr<std::vector<std::vector<double>>> &matrix_ptr)
        : data_(matrix_ptr)
    {}

    // 从输入流构造
    // @param stream 输入流
    BasicData(std::istream &stream,
              std::size_t row_number,
              std::size_t col_number);

    // 拷贝构造函数
    BasicData(const BasicData &data) = default;

    // 移动构造函数
    BasicData(BasicData &&data) = default;

    // 拷贝赋值函数
    BasicData &operator=(const BasicData &data) = default;

    // 析构函数
    virtual ~BasicData() = default;

    // 获取数据矩阵的拷贝
    const std::vector<std::vector<double>> &get_data() const { return *data_; }

    // 获取数据矩阵的引用
    std::vector<std::vector<double>> &data() { return *data_; }

    // 重新设置大小
    // @param row_number 行数
    // @param col_number 列数
    // @param init_value 初始化值
    void resize(const std::size_t &row_number,
                const std::size_t &col_number,
                const double &init_value = 0.0);

    // 获取行数
    std::size_t get_row_number() const { return data_->front().size(); }
    // 获取列数
    std::size_t get_col_number() const { return data_->size(); }

    // 获取指定列的数据
    // @param col_index 列索引
    std::vector<double> get_col(const std::size_t &col_index) const
    {
        return data_->at(col_index);
    }

    // 获取指定行的数据
    // @param row_index 行索引
    std::vector<double> get_row(const std::size_t &row_index) const
    {
        std::vector<double> row_data;
        for (const auto &col : *data_)
        {
            row_data.push_back(col.at(row_index));
        }
        return row_data;
    }

    // 获取指定行列的引用
    // @param col_index 列索引
    std::vector<double> &col(const std::size_t &col_index)
    {
        return data_->at(col_index);
    }

    // 计算每一列信号的能量
    // @return 每一列信号的能量
    std::vector<double> Energy() const;

    // 计算每一列信号的正向最大值
    // @return 每一列信号的正最大值及其索引
    std::vector<std::pair<double, size_t>> PositiveMax() const;

    // 计算每一列信号的负向最大值
    // @return 每一列信号的负最大值及其索引
    std::vector<std::pair<double, size_t>> NegativeMax() const;

    // 计算每一列信号的绝对最大值
    // @return 每一列信号的绝对最大值及其索引
    std::vector<std::pair<double, size_t>> AbsoluteMax() const;

    // 写入数据流，输出数据矩阵。列主序，行为时程，列为节点
    // @param output_stream 输出流
    void WriteToStream(std::ostream &output_stream) const;

protected:
    // 数据矩阵：列主序，行为时程，列为节点
    std::shared_ptr<std::vector<std::vector<double>>> data_ =
        std::make_shared<std::vector<std::vector<double>>>();

    // 从指定大小构造
    // @param row_number 行数
    // @param col_number 列数
    // @param init_value 初始化值
    BasicData(const std::size_t &row_number,
              const std::size_t &col_number,
              const double &init_value = 0.0);
};

} // namespace data_structure

#endif // DATA_STRUCTURE_BASIC_DATA_H