/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\settings\config.cpp
** -----
** File Created: Sunday, 3rd November 2024 17:15:57
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Sunday, 3rd November 2024 20:53:58
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description: 定义了配置文件的结构体，用于读取和保存配置文件

// associated header
#include "config.h"

// stdc++ headers
#include <fstream>
#include <stdexcept>
#include <vector>

// third-party library headers
#include "nlohmann/json.hpp"

namespace settings
{

// 从JSON文件中读取配置
Config *Config::load_config(const std::string &config_file)
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
    data_sampling_frequency = config["DataConfig"]["sampling_frequency"];
    data_time_step = config["DataConfig"]["time_step"];
    data_direction = config["DataConfig"]["direction"];
    data_measurement_number = config["DataConfig"]["measurement_number"];
    data_scale = config["DataConfig"]["scale"];
    data_time_window = config["DataConfig"]["time_window"];
    data_window_step = config["DataConfig"]["window_step"];
    data_time_count = config["DataConfig"]["time_count"];

    // 建筑信息相关配置
    building_floor_height =
        config["BuildingConfig"]["floor_height"].get<std::vector<double>>();
    building_measurement_height = config["BuildingConfig"]["measurement_height"]
                                      .get<std::vector<double>>();
    building_measurement_index =
        config["BuildingConfig"]["measurement_index"].get<std::vector<int>>();

    // 地震动参数计算
    // 反应谱计算的参数
    gmp_damping_ratio = config["GmpConfig"]["damping_ratio"];
    gmp_max_period = config["GmpConfig"]["max_period"];
    gmp_period_step = config["GmpConfig"]["period_step"];
    // Fourier变换的参数
    gmp_max_frequency = config["GmpConfig"]["max_frequency"];
    // 地震动参数计算滤波处理
    gmp_filter_flag = config["GmpConfig"]["filter_flag"];
    gmp_filter_order = config["GmpConfig"]["filter_order"];
    gmp_low_frequency = config["GmpConfig"]["low_frequency"];
    gmp_high_frequency = config["GmpConfig"]["high_frequency"];
    gmp_filter_type = config["GmpConfig"]["filter_type"];
    gmp_filter_function = config["GmpConfig"]["filter_function"];
    gmp_filter_generator = config["GmpConfig"]["filter_generator"];

    // 工程需求参量计算
    edp_method = config["EdpConfig"]["method"];
    edp_filter_order = config["EdpConfig"]["filter_order"];
    edp_low_frequency = config["EdpConfig"]["low_frequency"];
    edp_high_frequency = config["EdpConfig"]["high_frequency"];
    edp_filter_type = config["EdpConfig"]["filter_type"];
    edp_filter_function = config["EdpConfig"]["filter_function"];
    edp_filter_generator = config["EdpConfig"]["filter_generator"];
    edp_interp_method = config["EdpConfig"]["interp_method"];

    // 安全评价计算
    safty_acc_threshold =
        config["SaftyConfig"]["acc_limits"].get<std::vector<double>>();
    safty_idr_threshold =
        config["SaftyConfig"]["idr_limits"].get<std::vector<double>>();

    return this;
}

// 保存配置文件
void Config::save_config(const std::string &config_file)
{
    // JSON配置文件
    nlohmann::json config;

    // 读取原始配置，防止丢失未更改项
    std::ifstream ifs(config_file);
    if (ifs.is_open())
    {
        ifs >> config;
        ifs.close();
    }

    // 写入配置
    config["DataConfig"]["sampling_frequency"] = data_sampling_frequency;
    config["DataConfig"]["time_step"] = data_time_step;
    config["DataConfig"]["direction"] = data_direction;
    config["DataConfig"]["measurement_number"] = data_measurement_number;
    config["DataConfig"]["scale"] = data_scale;
    config["DataConfig"]["time_window"] = data_time_window;
    config["DataConfig"]["window_step"] = data_window_step;
    config["DataConfig"]["time_count"] = data_time_count;

    // 建筑信息相关配置
    config["BuildingConfig"]["floor_height"] = building_floor_height;
    config["BuildingConfig"]["measurement_height"] =
        building_measurement_height;
    config["BuildingConfig"]["measurement_index"] = building_measurement_index;

    // 地震动参数计算
    // 反应谱计算的参数
    config["GmpConfig"]["damping_ratio"] = gmp_damping_ratio;
    config["GmpConfig"]["max_period"] = gmp_max_period;
    config["GmpConfig"]["period_step"] = gmp_period_step;
    // Fourier变换的参数
    config["GmpConfig"]["max_frequency"] = gmp_max_frequency;
    // 地震动参数计算滤波处理
    config["GmpConfig"]["filter_flag"] = gmp_filter_flag;
    config["GmpConfig"]["filter_order"] = gmp_filter_order;
    config["GmpConfig"]["low_frequency"] = gmp_low_frequency;
    config["GmpConfig"]["high_frequency"] = gmp_high_frequency;
    config["GmpConfig"]["filter_type"] = gmp_filter_type;
    config["GmpConfig"]["filter_function"] = gmp_filter_function;
    config["GmpConfig"]["filter_generator"] = gmp_filter_generator;

    // 工程需求参量计算
    config["EdpConfig"]["filter_order"] = edp_filter_order;
    config["EdpConfig"]["low_frequency"] = edp_low_frequency;
    config["EdpConfig"]["high_frequency"] = edp_high_frequency;
    config["EdpConfig"]["filter_type"] = edp_filter_type;
    config["EdpConfig"]["filter_function"] = edp_filter_function;
    config["EdpConfig"]["filter_generator"] = edp_filter_generator;
    config["EdpConfig"]["interp_method"] = edp_interp_method;

    // 安全评价计算
    config["SaftyConfig"]["acc_limits"] = safty_acc_threshold;
    config["SaftyConfig"]["idr_limits"] = safty_idr_threshold;

    // 写入文件
    std::ofstream ofs(config_file);
    ofs << config.dump(4);
    ofs.close();
}

} // namespace settings