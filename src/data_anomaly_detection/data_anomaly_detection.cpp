/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\data_anomaly_detection\data_anomaly_detection.cpp
** -----
** File Created: Monday, 5th August 2024 17:40:31
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Saturday, 10th August 2024 22:24:14
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description:
// 数据异常检测类的实现。

// associated header
#include "data_anomaly_detection.h"

// stdc++ headers
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <list>

// project headers
#include "data_structure/acceleration.h"
#include "numerical_algorithm/vector_calculation.h"

namespace data_anomaly_detection
{
// 从加速度信息构造
DataAnomalyDetection::DataAnomalyDetection(
    const data_structure::Acceleration &acceleration_data,
    int channel_num)
    : channel_num_(channel_num), result_(channel_num)
{
    for (std::size_t i = 0; i < acceleration_data.get_col_number(); ++i)
    {
        acceleration_data_[i] = acceleration_data.get_col(i);
    }
}

// 从加速度信息指针构造
DataAnomalyDetection::DataAnomalyDetection(
    const std::shared_ptr<data_structure::Acceleration> &acceleration_data_ptr,
    int channel_num)
    : channel_num_(channel_num), result_(channel_num)
{
    for (std::size_t i = 0; i < acceleration_data_ptr->get_col_number(); ++i)
    {
        acceleration_data_[i] = acceleration_data_ptr->get_col(i);
    }
}

// 从vector<vector<double>>构造
DataAnomalyDetection::DataAnomalyDetection(
    const std::vector<std::vector<double>> &acceleration_data,
    int channel_num)
    : channel_num_(channel_num), result_(channel_num)
{
    for (std::size_t i = 0; i < acceleration_data.size(); ++i)
    {
        acceleration_data_[i] = acceleration_data[i];
    }
}

// 处理数据异常问题
std::vector<std::vector<int>> DataAnomalyDetection::ProcessDataAnomaly()
{
    // 1.信号丢失检测
    if (signal_loss_detection())
    {
        throw std::runtime_error("Signal loss detected.");
    }

    // 2.计算信号能量
    std::list<double> energy;
    for (const auto &data : acceleration_data_)
    {
        energy.push_back(numerical_algorithm::CrossCorrelationAtShift(
            data.second, data.second, 0));
    }
    std::size_t measure_num = energy.size() / channel_num_;

    // 3.根据0偏移互相关值确定方向
    for (std::size_t direction = 0; direction != channel_num_; ++direction)
    {
        // 3.1寻找能量最小值作为计算初始点
        auto min_index = std::distance(
            energy.begin(), std::min_element(energy.begin(), energy.end()));
        result_[direction].push_back(min_index);
        // 低层某一方向的信号
        auto lower_data = acceleration_data_[min_index];
        // 移除已经确定方向的数据和能量序列
        acceleration_data_.erase(min_index);
        energy.erase(std::next(energy.begin(), min_index));
        // 3.2寻找当前方向的所有层信号
        for (std::size_t lower_mea = 1; lower_mea != measure_num; ++lower_mea)
        {
            // 3.2.1计算归一化互相关值
            std::vector<double> cross_correlation;
            for (const auto &data : acceleration_data_)
            {
                cross_correlation.push_back(
                    numerical_algorithm::CrossCorrelationAtShiftNormalized(
                        lower_data, data.second, 0));
            }
            // 3.2.2寻找最大值作为方向
            auto max_iter = std::max_element(cross_correlation.begin(),
                                             cross_correlation.end()),
                 min_iter = std::min_element(cross_correlation.begin(),
                                             cross_correlation.end());
            int max_index = 0;
            if (std::abs(*max_iter) > std::abs(*min_iter))
            {
                max_index = std::distance(cross_correlation.begin(), max_iter);
            }
            else
            {
                max_index = -std::distance(cross_correlation.begin(), min_iter);
            }
            // 3.2.3结果赋值，更新低层，移除已经确定方向的数据和能量序列
            result_[direction].push_back(max_index);
            lower_data = acceleration_data_[max_index];
            acceleration_data_.erase(max_index);
            energy.erase(std::next(energy.begin(), max_index));
        }
    }
    return result_;
}

} // namespace data_anomaly_detection