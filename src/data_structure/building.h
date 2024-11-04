﻿/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\data_structure\building.h
** -----
** File Created: Monday, 15th July 2024 00:51:05
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Saturday, 10th August 2024 22:25:08
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 建筑信息类

// TODO:建筑信息类Building的设置方式

#ifndef DATA_STRUCTURE_BUILDING_H
#define DATA_STRUCTURE_BUILDING_H

// stdc++ headers
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// project headers
#include "settings/config.h"

namespace data_structure
{

// 建筑信息类
class Building
{
public:
    // 构造函数
    Building() = default;

    // 从std::vector<double>构造
    Building(const std::vector<double> &measuren_height,
             const std::vector<double> &floor_height)
        : measure_height_(measuren_height), floor_height_(floor_height)
    {
        calculate_inter_height();
    }

    // 从配置文件构造
    Building(std::shared_ptr<settings::Config> config) { LoadConfig(config); }

    // 从配置获取建筑信息
    void LoadConfig(std::shared_ptr<settings::Config> config)
    {
        if (config == nullptr)
        {
            throw std::invalid_argument("config is nullptr");
        }

        measure_height_ = config->building_measurement_height;
        floor_height_ = config->building_floor_height;
        measure_index_ = config->building_measurement_index;
        calculate_inter_height();
    }

    // 析构函数
    ~Building() = default;

    // 获取建筑测点高度
    std::vector<double> &get_measuren_height() { return measure_height_; }

    // 获取建筑楼层高度
    std::vector<double> &get_floor_height() { return floor_height_; }

    // 获取建筑层间高度
    std::vector<double> &get_inter_height() { return inter_height_; }

    // 获取有测点的楼层索引
    std::vector<int> &get_measure_index() { return measure_index_; }

    // 获取建筑测点数量
    size_t get_measuren_number() { return measure_height_.size(); }

    // 获取建筑楼层数量
    size_t get_floor_number() { return floor_height_.size(); }

private:
    // 建筑测点高度坐标
    std::vector<double> measure_height_{};
    // 建筑楼层高度
    std::vector<double> floor_height_{};
    // 建筑层间高度
    std::vector<double> inter_height_{};
    // 有测点的楼层索引
    std::vector<int> measure_index_{};

    // 计算层间高度
    void calculate_inter_height();
};

} // namespace data_structure

#endif // DATA_STRUCTURE_BUILDING_H