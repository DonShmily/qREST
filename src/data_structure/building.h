/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/data_structure/building.h
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
// Description:
// 建筑信息类

// TODO:建筑信息类Building的设置方式

#ifndef SRC_DATA_STRUCTURE_BUILDING_H_
#define SRC_DATA_STRUCTURE_BUILDING_H_

// stdc++ headers
#include <vector>

// third-party library headers

// project-specific headers

namespace data_structure
{

// 建筑信息类
class Building
{
public:
    // 构造函数
    Building() = default;
    // 拷贝构造函数
    Building(const Building &building_message) = default;
    // 移动构造函数
    Building(Building &&building_message) = default;
    // 析构函数
    ~Building() = default;

    // 设置建筑信息
    void set_building_message() {}

    // 获取建筑测点高度
    std::vector<double> &get_measuren_height() { return measuren_height_; }
    // 获取建筑楼层高度
    std::vector<double> &get_floor_height() { return floor_height_; }
    // 获取建筑层间高度
    std::vector<double> &get_inter_height() { return inter_height_; }

private:
    // 建筑测点高度坐标
    std::vector<double> measuren_height_{};
    // 建筑楼层高度
    std::vector<double> floor_height_{};
    // 建筑层间高度
    std::vector<double> inter_height_{};
};

} // namespace data_structure

#endif // SRC_DATA_STRUCTURE_BUILDING_H_