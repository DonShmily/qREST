/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\qrest_calculation\data_interface.cpp
** -----
** File Created: Monday, 26th August 2024 10:47:06
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Monday, 4th November 2024 01:17:46
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description: 数据接口类的实现
// TODO: 实现串口通信

// associated header
#include "data_interface.h"

// stdc++ library
#include <cstddef>
#include <fstream>
#include <sstream>
#include <vector>

// third-party library
#include "data_structure/acceleration.h"

// 读取文件的构造函数
DataInterface::DataInterface(const std::string &file_path)
{
    ReadFile(file_path);
    LoadConfig();
}

// 读取串口的构造函数
DataInterface::DataInterface(const std::string &port, const int &baudrate)
{
    ReadSerialPort(port, baudrate);
    LoadConfig();
}

// 读取文件
void DataInterface::ReadFile(const std::string &file_path)
{
    // 读取整个文件则将窗口设为时间总数
    data_config_.time_window_ = data_config_.time_count_;

    std::ifstream file(file_path);
    if (!file.is_open())
    {
        throw std::runtime_error("File not found");
    }

    // 读取数据
    ori_acc.resize(data_config_.direction_ * data_config_.mea_number_);
    std::string line;
    while (std::getline(file, line))
    // for (int i = time_idx_; i < time_idx_ + data_config_.time_window; i++)
    {
        // std::getline(file, line);
        std::istringstream iss(line);
        double value;
        for (size_t i = 0;
             i < data_config_.direction_ * data_config_.mea_number_;
             i++)
        {
            iss >> value;
            ori_acc.at(i).push_back(value);
        }
    }

    // 将数据转换为加速度数据
    acc_.clear();
    for (size_t i = 0; i < data_config_.direction_; i++)
    {
        // 从指定列构造，这里每1/3列构造一个方向的数据
        std::vector<size_t> col_idx;
        for (size_t j = 0; j < data_config_.mea_number_; j++)
        {
            col_idx.push_back(j + data_config_.mea_number_ * i);
        }
        acc_.emplace_back(
            ori_acc, col_idx, data_config_.frequency_, data_config_.scale_);
    }

    // 修正数据
    // TODO:地震动事件识别
    // ModifyAcceleration();
}

// 根据时间窗口读取文件
void DataInterface::ReadFileTimeWindow(const std::string &file_path)
{
    if (acc_.empty())
    {
        std::ifstream file(file_path);
        if (!file.is_open())
        {
            throw std::runtime_error("File not found");
        }

        // 读取数据
        ori_acc.resize(data_config_.direction_ * data_config_.mea_number_);
        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            double value;
            for (size_t i = 0;
                 i < data_config_.direction_ * data_config_.mea_number_;
                 i++)
            {
                iss >> value;
                ori_acc.at(i).push_back(value);
            }
        }
    }

    std::vector<std::vector<double>> ori_acc_part;
    for (size_t i = 0; i != ori_acc.size(); i++)
    {
        ori_acc_part.push_back(std::vector<double>(
            ori_acc[i].begin() + time_idx_,
            ori_acc[i].begin() + time_idx_ + data_config_.time_window_));
    }
    time_idx_ += data_config_.window_step_;

    // 将数据转换为加速度数据
    acc_.clear();
    for (size_t i = 0; i < data_config_.direction_; i++)
    {
        // 从指定列构造，这里每1/3列构造一个方向的数据
        std::vector<size_t> col_idx;
        for (size_t j = 0; j < data_config_.mea_number_; j++)
        {
            col_idx.push_back(j + data_config_.mea_number_ * i);
        }
        acc_.emplace_back(ori_acc_part,
                          col_idx,
                          data_config_.frequency_,
                          data_config_.scale_);
    }
}

// 读取串口
void DataInterface::ReadSerialPort(const std::string &port, const int &baudrate)
{
    // 串口通信
    // 未实现
}

// 获取加速度数据
data_structure::Acceleration
DataInterface::GetAccelerationData(std::size_t index) const
{
    return acc_.at(index);
}

// 加载配置
void DataInterface::LoadConfig(const std::string &config_file)
{
    config_ = std::make_shared<settings::Config>(config_file);
    building_ = std::make_shared<data_structure::Building>(config_);

    data_config_.frequency_ = config_->data_sampling_frequency;
    data_config_.scale_ = config_->data_scale;
    data_config_.direction_ = config_->data_direction;
    data_config_.mea_number_ = config_->data_measurement_number;
    data_config_.time_window_ = config_->data_time_window;
    data_config_.window_step_ = config_->data_window_step;
    data_config_.time_count_ = config_->data_time_count;
}

// 修正加速度数据
void DataInterface::ModifyAcceleration()
{
    // 阈值触发，按照最底层触发阈值判断地震动事件
    // 任一方向的底部测点超过0.1gal则视为触发
    size_t t = 0;
    while (true)
    {
        bool trigger = false;
        if (trigger || t == data_config_.time_count_)
        {
            break;
        }
        for (size_t i = 0; i < data_config_.direction_; i++)
        {
            if (acc_[i].data()[0][t] > 1e-3)
            {
                trigger = true;
                break;
            }
        }
        ++t;
    }

    // 修正数据
    auto new_acc =
        std::vector<data_structure::Acceleration>(data_config_.direction_);
    for (size_t i = 0; i < data_config_.direction_; i++)
    {
        new_acc[i].data().resize(data_config_.mea_number_);
        for (size_t j = 0; j < data_config_.mea_number_; j++)
        {
            new_acc[i].data()[j] = std::vector<double>(
                acc_[i].data()[j].begin() + t, acc_[i].data()[j].end());
        }
    }
    acc_ = new_acc;
}