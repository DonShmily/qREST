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
#include <iosfwd>
#include <vector>

// third-party library headers
#include "eigen3/Eigen/Core"


namespace data_structure
{

class BasicData
{
public:
    // 默认构造函数
    BasicData() = default;
    // 从二维std::vector<std::vector<double>>构造
    // @param matrix 数据矩阵
    BasicData(const std::vector<std::vector<double>> &matrix);
    // 从输入流构造
    // @param stream 输入流
    BasicData(std::istream &stream,
              std::size_t row_number,
              std::size_t col_number);
    // 从Eigen::MatrixXd构造
    // @param matrix 数据矩阵
    BasicData(const Eigen::MatrixXd &matrix) : data_(matrix){};
    // 拷贝构造函数
    BasicData(const BasicData &data) = default;
    // 移动构造函数
    BasicData(BasicData &&data) = default;
    // 析构函数
    virtual ~BasicData() = default;

    // 获取数据矩阵的拷贝
    const Eigen::MatrixXd &get_data() const { return data_; }

    // 获取数据矩阵的引用
    Eigen::MatrixXd &data() { return data_; }

    // 写入数据流，输出数据矩阵。列主序，行为时程，列为节点
    // @param output_stream 输出流
    void WriteToStream(std::ostream &output_stream) const;

protected:
    // 数据矩阵：列主序，行为时程，列为节点
    Eigen::MatrixXd data_;

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