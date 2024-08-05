/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/data_anomaly_detection/data_anomaly_detection.cpp
// Date: 2024-7
// Author: 董飞跃(Dong Fieyue)
// Description:
// 数据异常检测类，用于检测数据中的异常，信号丢失、方向错误等。提供基础功能，如数据预处理、异常检测等。

// associated header
#include "data_anomaly_detection.h"

// stdc++ headers
#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

// project headers
#include "data_structure/acceleration.h"
#include "numerical_algorithm/vector_calculation.h"


namespace data_anomaly_detection
{

// 处理数据异常问题
std::vector<data_structure::Acceleration>
DataAnomalyDetection::ProcessDataAnomaly()
{
    // 计算所有通道的能量
    std::vector<double> energy = acceleration_data_.Energy();
    std::size_t measure_num = energy.size() / channel_num_;
    // 初始化结果
    for (std::size_t i = 0; i != channel_num_; ++i)
    {
        result_[i].data().resize(acceleration_data_.get_col_number());
    }

    // 1.信号丢失检测
    if (signal_loss_detection(energy))
    {
        throw std::runtime_error("Signal loss detected.");
    }

    // 2.根据信号能量排序测点
    // 这一段写得我快吐了，又不想对数据结构部分作大修改，只能这样写了
    // 2.1计算每个测点的信号能量之和
    std::vector<double> measure_energy(measure_num);
    for (std::size_t i = 0; i != measure_num; ++i)
    {
        for (std::size_t j = 0; j != channel_num_; ++j)
        {
            measure_energy[i] += energy[i * channel_num_ + j];
        }
    }
    // 2.2根据各层能量和排序所有层，实际排序运算的次数很少，所以使用了简单的冒泡排序
    for (std::size_t i = 0; i != measure_num - 1; ++i)
    {
        bool swapped = false;
        for (std::size_t j = 0; j != measure_num - i - 1; ++j)
        {
            if (measure_energy[j] < measure_energy[j + 1])
            {
                for (std::size_t k = 0; k != channel_num_; ++k)
                {
                    std::swap(energy[j * channel_num_ + k],
                              energy[(j + 1) * channel_num_ + k]);
                    std::swap(
                        acceleration_data_.data()[j * channel_num_ + k],
                        acceleration_data_.data()[(j + 1) * channel_num_ + k]);
                }
                swapped = true;
            }
        }
        if (!swapped)
        {
            break;
        }
    }

    // 3.方向错误检测
    // 3.1对最底层排序，确定UD方向
    for (std::size_t i = 0; i != channel_num_ - 1; ++i)
    {
        if (energy[i] > energy[i + 1])
        {
            std::swap(energy[i], energy[i + 1]);
            std::swap(acceleration_data_.data()[i],
                      acceleration_data_.data()[i + 1]);
        }
    }
    // 对各方向信号的底层赋值
    for (std::size_t i = 0; i != channel_num_; ++i)
    {
        result_[i].data()[0] = acceleration_data_.get_col(i);
    }
    // 3.2对个方向的其余层赋值
    for (std::size_t direction = 0; direction != channel_num_; ++direction)
    {
        for (std::size_t i = 1; i != measure_num; ++i)
        {
            double max_correlation = 0;
            std::size_t max_index = 0;
            for (std::size_t j = 0; j != channel_num_; ++j)
            {
                double correlation =
                    numerical_algorithm::CrossCorrelationAtShift(
                        acceleration_data_.get_col((i - 1) * channel_num_
                                                   + direction),
                        acceleration_data_.get_col(i * channel_num_ + j),
                        0);
                if (correlation > max_correlation)
                {
                    max_correlation = correlation;
                    max_index = j;
                }
            }
            result_[direction].data()[i] =
                acceleration_data_.get_col(i * channel_num_ + max_index);
        }
    }
    // 一种基于信号能量剔除UD方向的方法，理论上应当由更好的计算速度，但是已经有了更加通用的方法，故舍弃
    // 如果是三通道，则寻找所有测点能量最低的信号作为UD方向
    /*
    if (channel_num_ == 3)
    {
        for (std::size_t i = 0; i != measure_num; ++i)
        {
            for (std::size_t j = 0; j != channel_num_ - 1; ++j)
            {
                if (energy[i * measure_num + j]
                    > energy[i * measure_num + j + 1])
                {
                    std::swap(energy[i * measure_num + j],
                              energy[i * measure_num + j + 1]);
                    std::swap(
                        acceleration_data_.data()[i * measure_num + j],
                        acceleration_data_.data()[i * measure_num + j + 1]);
                }
            }
            result_[0].data().push_back(
                acceleration_data_.get_col(i * channel_num_));
        }
    }
    */
    return result_;
}

// 信号丢失检测
bool DataAnomalyDetection::signal_loss_detection(
    const std::vector<double> &energy)
{
    // TODO: 信号丢失检测
    return false;
}

} // namespace data_anomaly_detection