/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\settings\config.h
** -----
** File Created: Sunday, 3rd November 2024 17:15:49
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Sunday, 3rd November 2024 20:53:50
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description: 定义了配置文件的结构体，用于读取和保存配置文件

#ifndef CONFIG_H
#define CONFIG_H

// stdc++ headers
#include <string>
#include <vector>

namespace settings
{
// 配置文件结构体
struct Config
{
    // 构造函数
    Config() = default;

    // 从JSON文件构造
    Config(const std::string &config_file) { load_config(config_file); }

    // 数据采集相关配置
    double data_sampling_frequency; // 采样频率
    double data_time_step;          // 时间步长
    int data_direction;             // 方向数量
    int data_measurement_number;    // 测点数量
    double data_scale;              // 调幅系数
    int data_time_window;           // 时间窗口长度
    int data_window_step;           // 时间窗口步长
    int data_time_count;            // 时间点数量

    // 建筑信息相关配置
    std::vector<double> building_floor_height;       // 楼层高度
    std::vector<double> building_measurement_height; // 测点高度
    std::vector<int> building_measurement_index;     // 测点坐在楼层

    // 地震动参数计算
    // 反应谱计算的参数
    double gmp_damping_ratio; // 阻尼比
    double gmp_max_period;    // 最大周期
    double gmp_period_step;   // 周期步长
    // Fourier变换的参数
    double gmp_max_frequency; // 最大频率
    // 地震动参数计算滤波处理
    bool gmp_filter_flag;      // 是否滤波
    int gmp_filter_order;      // 滤波器阶数
    double gmp_low_frequency;  // 低频截止频率
    double gmp_high_frequency; // 高频截止频率
    int gmp_filter_type;       // 滤波器类型
    int gmp_filter_function;   // 滤波函数
    int gmp_filter_generator;  // 滤波器生成器

    // 工程需求参量计算
    int edp_method;            // 工程需求参量计算方法
    int edp_filter_order;      // 滤波器阶数
    double edp_low_frequency;  // 低频截止频率
    double edp_high_frequency; // 高频截止频率
    int edp_filter_type;       // 滤波器类型
    int edp_filter_function;   // 滤波函数
    int edp_filter_generator;  // 滤波器生成器
    int edp_interp_method;     // 插值方法

    // 安全评价计算
    std::vector<double> safty_acc_threshold; // 安全评价加速度阈值
    std::vector<double> safty_idr_threshold; // 安全评价IDR阈值

    // 读取配置文件
    Config *load_config(const std::string &config_file);

    // 保存配置文件
    void save_config(const std::string &config_file);

}; // struct Config
} // namespace settings


#endif // CONFIG_H