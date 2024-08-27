/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\data_anomaly_detection\data_anomaly_detection.h
** -----
** File Created: Monday, 5th August 2024 17:40:31
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Saturday, 10th August 2024 22:24:41
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 数据异常检测类，用于检测数据中的异常，信号丢失、方向错误等。提供基础功能，如数据预处理、异常检测等。

#ifndef DATA_ANOMALY_DETECTION_H
#define DATA_ANOMALY_DETECTION_H

// stdc++ headers
#include <cstddef>
#include <map>
#include <memory>
#include <vector>

// project headers
#include "data_structure/acceleration.h"
#include "data_structure/basic_data_structure.h"

namespace data_anomaly_detection
{
class DataAnomalyDetection
{
public:
    // 默认构造函数
    DataAnomalyDetection() = default;

    // 从加速度信息构造
    // @param acceleration_data 加速度信息
    DataAnomalyDetection(const data_structure::Acceleration &acceleration_data,
                         int channel_num = 3);

    // 从加速度信息指针构造
    // @param acceleration_data_ptr 加速度信息指针
    DataAnomalyDetection(const std::shared_ptr<data_structure::Acceleration>
                             &acceleration_data_ptr,
                         int channel_num = 3);

    // 从vector<vector<double>>构造
    // @param acceleration_data 加速度信息
    // @param channel_num 通道数
    DataAnomalyDetection(
        const std::vector<std::vector<double>> &acceleration_data,
        int channel_num = 3);

    // 处理数据异常问题
    // @return 处理完成后的加速度异常信息，根据通道数生成的vector<int>
    std::vector<std::vector<int>> ProcessDataAnomaly();

private:
    // 加速度信息
    std::map<int, std::vector<double>> acceleration_data_;

    std::vector<std::vector<int>> result_;

    // 通道数
    int channel_num_;

    // 信号丢失检测（根据信号能量判断）
    // @param energy 信号能量
    // @return 是否检测到信号丢失
    bool signal_loss_detection();
};
} // namespace data_anomaly_detection


#endif // DATA_ANOMALY_DETECTION_H