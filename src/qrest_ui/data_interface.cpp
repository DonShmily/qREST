﻿/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\qrest_ui\data_interface.cpp
** -----
** File Created: Monday, 26th August 2024 10:47:06
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Monday, 26th August 2024 10:58:27
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
#include "nlohmann/json.hpp"

// 读取文件的构造函数
DataInterface::DataInterface(const std::string &file_path)
{
    ReadFile(file_path);
    LoadBuilding();
}

// 读取串口的构造函数
DataInterface::DataInterface(const std::string &port, const int &baudrate)
{
    ReadSerialPort(port, baudrate);
    LoadBuilding();
}

// 读取文件
void DataInterface::ReadFile(const std::string &file_path)
{
    std::ifstream file(file_path);
    if (!file.is_open())
    {
        throw std::runtime_error("File not found");
    }

    // 读取数据
    std::vector<std::vector<double>> ori_acc;
    ori_acc.resize(config_.direction_ * config_.mea_number_);
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        double value;
        for (size_t i = 0; i < config_.direction_ * config_.mea_number_; i++)
        {
            iss >> value;
            ori_acc.at(i).push_back(value);
        }
    }

    // 将数据转换为加速度数据
    acc_.clear();
    for (size_t i = 0; i < config_.direction_; i++)
    {
        // 从指定列构造，这里每1/3列构造一个方向的数据
        std::vector<size_t> col_idx;
        for (size_t j = 0; j < config_.mea_number_; j++)
        {
            col_idx.push_back(j + config_.mea_number_ * i);
        }
        acc_.emplace_back(ori_acc, col_idx, config_.frequency_, config_.scale_);
    }

    // 修正数据
    // ModifyAcceleration();
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
    // JSON配置文件
    nlohmann::json config;
    std::ifstream ifs(config_file);
    if (ifs.is_open())
    {
        ifs >> config;
        ifs.close();
    }
    else
    {
        throw std::runtime_error("Cannot open the configuration file.");
    }

    // 从JSON中读取配置
    config_.direction_ = config["DataConfig"]["direction"];
    config_.mea_number_ = config["DataConfig"]["measurement_number"];
    config_.time_count_ = config["DataConfig"]["time_count"];
    config_.frequency_ = config["DataConfig"]["frequency"];
    config_.scale_ = config["DataConfig"]["scale"];
}

// 获取建筑信息
void DataInterface::LoadBuilding(const std::string &building_file)
{
    building_.LoadConfig(building_file);
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
        if (trigger || t == config_.time_count_)
        {
            break;
        }
        for (size_t i = 0; i < config_.direction_; i++)
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
    auto new_acc = std::vector<data_structure::Acceleration>(3);
    for (size_t i = 0; i < config_.direction_; i++)
    {
        for (size_t j = 0; j < config_.mea_number_; j++)
        {
            new_acc[i].data()[j] = std::vector<double>(
                acc_[i].data()[j].begin() + t, acc_[i].data()[j].end());
        }
    }
    acc_ = new_acc;
}