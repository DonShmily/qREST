/**
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
#include <string>
#include <vector>

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

    // 析构函数
    ~Building() = default;

    // 从配置文件读取建筑信息
    void
    LoadConfig(const std::string &config_file = "config/Building_Info.json");

    // 设置建筑信息
    void set_building_message() {}

    // 获取建筑测点高度
    std::vector<double> &get_measuren_height() { return measure_height_; }

    // 获取建筑楼层高度
    std::vector<double> &get_floor_height() { return floor_height_; }

    // 获取建筑层间高度
    std::vector<double> &get_inter_height() { return inter_height_; }

private:
    // 建筑测点高度坐标
    std::vector<double> measure_height_{};
    // 建筑楼层高度
    std::vector<double> floor_height_{};
    // 建筑层间高度
    std::vector<double> inter_height_{};

    // 计算层间高度
    void calculate_inter_height();
};

} // namespace data_structure

#endif // DATA_STRUCTURE_BUILDING_H