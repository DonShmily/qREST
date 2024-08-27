/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\data_structure\building.cpp
** -----
** File Created: Monday, 15th July 2024 00:51:18
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Saturday, 10th August 2024 22:25:04
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 建筑信息类的实现

// associated header
#include "building.h"

// stdc++ headers
#include <fstream>
#include <vector>

// third-party library headers
#include "nlohmann/json.hpp"

namespace data_structure
{
// 从配置文件读取
void Building::LoadConfig(const std::string &config_file)
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

    // 读取配置
    floor_height_ =
        config["Building"]["floor_height"].get<std::vector<double>>();
    measuren_height_ =
        config["Building"]["measuren_height"].get<std::vector<double>>();
}

// 计算建筑层间高度
void Building::calculate_inter_height()
{
    inter_height_.resize(floor_height_.size() - 1);
    for (size_t i = 0; i < inter_height_.size(); ++i)
    {
        inter_height_[i] = floor_height_[i + 1] - floor_height_[i];
    }
}

} // namespace data_structure