/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\qrest_ui\data_interface.h
** -----
** File Created: Monday, 26th August 2024 10:46:57
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Monday, 26th August 2024 10:48:08
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 定义了数据接口类，用于获取数据，读取文件或者串口通信，产出数据为Acceleration类型

#ifndef DATA_INTERFACE_H
#define DATA_INTERFACE_H

// stdc++ headers
#include <cstddef>
#include <string>
#include <vector>

// project headers
#include "data_structure/acceleration.h"
#include "data_structure/building.h"

// 数据接口类的配置结构体
struct DataInterfaceConfig
{
    // 方向数量
    size_t direction_ = 3;
    // 测点数量
    size_t mea_number_ = 9;
    // 时间点计数
    size_t time_count_ = 30000;
    // 采样频率
    double frequency_ = 50;
    // 调幅系数
    double scale_ = 0.01;
}; // struct DataInterfaceConfig

// 数据接口类，用于获取数据，读取文件或者串口通信，产出数据为Acceleration和Building类型
class DataInterface
{
public:
    // 构造函数
    DataInterface() = default;

    // 读取文件的构造函数
    // @param file_path 文件路径
    DataInterface(const std::string &file_path);

    // 读取串口的构造函数
    // @param port 串口号
    // @param baudrate 波特率
    DataInterface(const std::string &port, const int &baudrate);

    // 析构函数
    ~DataInterface() = default;

    // 读取文件
    // @param file_path 文件路径
    void ReadFile(const std::string &file_path);

    // 读取串口
    // @param port 串口号
    // @param baudrate 波特率
    void ReadSerialPort(const std::string &port, const int &baudrate);

    // 获取加速度数据
    // @param index 方向索引
    data_structure::Acceleration GetAccelerationData(std::size_t index) const;

    // 加载配置
    // @param config_file 配置文件路径
    void LoadConfig(const std::string &config_file = "config/Data_Config.json");

    // 获取建筑信息
    // @param config_file 建筑信息文件路径
    void LoadBuilding(
        const std::string &building_file = "config/Building_Info.json");

private:
    // 读取得到的加速度数据
    std::vector<data_structure::Acceleration> acceleration_data_;

    // 读取得到的建筑信息
    data_structure::Building building_;

    // 数据接口配置
    DataInterfaceConfig config_;
};

#endif // DATA_INTERFACE_H