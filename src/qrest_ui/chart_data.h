/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\qrest_ui\chart_data.h
** -----
** File Created: Monday, 26th August 2024 09:35:07
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Monday, 26th August 2024 21:23:31
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description: 定义了图表数据类，用于调用其他类，获取绘图数据
// 该类的主要功能是接收加速度数据和建筑信息，计算得到各种结果，将其转化为QSeries数据，提供给QChart进行绘图

#ifndef CHART_DATA_H
#define CHART_DATA_H

// stdc++ headers
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

// Qt headers
#include "QtCore/qlist.h"
#include "QtCore/qpoint.h"

// project headers
#include "data_interface.h"
#include "data_structure/acceleration.h"
#include "data_structure/building.h"
#include "edp_calculation/basic_edp_calculation.h"
#include "edp_calculation/filtering_integral.h"
#include "edp_calculation/modified_filtering_integral.h"
#include "gmp_calculation/gmp_calculation.h"
#include "safty_tagging/based_on_inter_story_drift.h"


// 图表数据类
class ChartData
{
public:
    using points_vector = std::pair<std::vector<double>, std::vector<double>>;

    // 构造函数
    ChartData() = default;

    // 构造函数
    // @param data_interface 数据接口对象
    ChartData(std::shared_ptr<DataInterface> data_interface,
              std::size_t dir = 0)
        : data_interface_(data_interface)
    {
        set_direction(dir);
        fi_.resize(data_interface_->config_.direction_);
        mfi_.resize(data_interface_->config_.direction_);
        safty_.resize(data_interface_->config_.direction_);
    }

    // 析构函数
    ~ChartData() = default;

    // 转换point_vector为QList<QPoint>指针的静态函数
    // @param points 数据序列，points_vector类型
    // @return QList<QPoint>指针
    static std::unique_ptr<QList<QPointF>>
    PointsVector2QList(const points_vector &points);

    // 设置当前方向
    // @param dir 方向
    void set_direction(size_t dir)
    {
        cur_dir_ = dir;
        clear();
    }

    /** 获取绘图点 **/

    // 获取加速度数据
    // @param idx 测点索引
    // @return 加速度数据序列指针
    points_vector get_acceleration(std::size_t idx);

    // 获取速度数据
    // @param idx 测点索引
    // @return 速度数据序列指针
    points_vector get_velocity(std::size_t idx);

    // 获取位移数据
    // @param idx 测点索引
    // @return 位移数据序列指针
    points_vector get_displacement(std::size_t idx);

    // 获取Sa数据
    // @param idx 测点索引
    // @return Sa数据序列指针
    points_vector get_sa(std::size_t idx);

    // 获取Sv数据
    // @param idx 测点索引
    // @return Sv数据序列指针
    points_vector get_sv(std::size_t idx);

    // 获取Sd数据
    // @param idx 测点索引
    // @return Sd数据序列指针
    points_vector get_sd(std::size_t idx);

    // 获取幅值谱数据
    // @param idx 测点索引
    // @return 幅值谱数据序列指针
    points_vector get_amplitude(std::size_t idx);

    // 获取功率谱数据
    // @param idx 测点索引
    // @return 相位谱数据序列指针
    points_vector get_power(std::size_t idx);

    // 获取FilteringIntegral指定楼层层间位移角时程数据
    // @param idx 楼层索引
    // @return 层间位移角时程数据序列指针
    points_vector get_fi_idr(std::size_t idx);

    // 获取FilteringIntegral指定楼层位移时程数据
    // @param idx 楼层索引
    // @return 楼层位移时程数据序列指针
    points_vector get_fi_disp(std::size_t idx);

    // 获取FilteringIntegral层间位移角分布数据
    // @param idx 楼层索引
    // @return 层间位移角分布数据序列指针
    points_vector get_fi_all_idr();

    // 获取ModifiedFilteringIntegral指定楼层层间位移角时程数据
    // @param idx 楼层索引
    // @return 层间位移角时程数据序列指针
    points_vector get_mfi_idr(std::size_t idx);

    // 获取ModifiedFilteringIntegral指定楼层位移时程数据
    // @param idx 楼层索引
    // @return 楼层位移时程数据序列指针
    points_vector get_mfi_disp(std::size_t idx);

    // 获取ModifiedFilteringIntegral层间位移角分布数据
    // @return 层间位移角分布数据序列指针
    points_vector get_mfi_all_idr();

private:
    /** 计算对象的数据成员 **/

    // 输入数据的数据接口对象
    std::shared_ptr<DataInterface> data_interface_{}; // 数据接口对象

    // 计算对象的索引
    size_t cur_dir_ = 0; // 当前方向
    size_t cur_idx_ = 0; // 当前测点索引

    // 计算结果对象成员
    bool gmp_calculated_;                                  // GMP是否已计算
    gmp_calculation::GmpCalculation gmp_{};                // GMP计算对象
    std::vector<edp_calculation::FilteringIntegral> fi_{}; // 滤波积分计算对象
    std::vector<edp_calculation::ModifiedFilteringIntegral>
        mfi_{}; // 改进滤波积分计算对象
    std::vector<safty_tagging::BasedOnInterStoryDrift> safty_{}; // 安全评估对象

    // 计算结果的私有函数
    void CalculateGmp(std::size_t idx); // 计算指定测点的GMP
    void CalculateEdpFi();              // 计算滤波积分
    void CalculateEdpMfi();             // 计算改进滤波积分
    void CalculateSafty();              // 计算安全评估

    // 改变计算对象后清除计算结果
    void clear();

    /** 绘图数据成员 **/

    // 时程数据相关的成员变量
    std::vector<double> time_{}; // 时间数据
    void get_time_();            // 生成时间横轴的函数

    // 反应谱数据相关的成员变量
    std::vector<double> period_{}; // 周期数据
    void get_period_();            // 生成周期横轴的函数

    // Fourier谱数据相关的成员变量
    std::vector<double> freq_{}; // 频率数据
    void get_freq_();            // 生成频域横轴的函数
};

#endif // CHART_DATA_H