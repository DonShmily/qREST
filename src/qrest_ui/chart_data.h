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
#include <vector>

// Qt headers
#include <QtCharts/QtCharts>

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
    // 构造函数
    ChartData() = default;

    // 构造函数
    // @param acc 单个方向的加速度数据
    // @param building 建筑信息
    ChartData(const data_structure::Acceleration &acc,
              const data_structure::Building &building)
        : acc_(acc), building_(building)
    {}

    // 析构函数
    ~ChartData() = default;

    // 设置加速度数据
    // @param acc 单个方向的加速度数据
    void set_acceleration(const data_structure::Acceleration &acc)
    {
        acc_ = acc;
        clear();
    }

    // 设置建筑信息
    // @param building 建筑信息
    void set_building(const data_structure::Building &building)
    {
        building_ = building;
        clear();
    }

    // 获取加速度数据
    // @param idx 测点索引
    // @return 加速度数据序列指针
    QLineSeries *get_acceleration(std::size_t idx);

    // 获取速度数据
    // @param idx 测点索引
    // @return 速度数据序列指针
    QLineSeries *get_velocity(std::size_t idx);

    // 获取位移数据
    // @param idx 测点索引
    // @return 位移数据序列指针
    QLineSeries *get_displacement(std::size_t idx);

    // 获取Sa数据
    // @param idx 测点索引
    // @return Sa数据序列指针
    QLineSeries *get_sa(std::size_t idx);

    // 获取Sv数据
    // @param idx 测点索引
    // @return Sv数据序列指针
    QLineSeries *get_sv(std::size_t idx);

    // 获取Sd数据
    // @param idx 测点索引
    // @return Sd数据序列指针
    QLineSeries *get_sd(std::size_t idx);

    // 获取幅值谱数据
    // @param idx 测点索引
    // @return 幅值谱数据序列指针
    QLineSeries *get_amplitude(std::size_t idx);

    // 获取功率谱数据
    // @param idx 测点索引
    // @return 相位谱数据序列指针
    QLineSeries *get_power(std::size_t idx);

    // 获取FilteringIntegral指定楼层层间位移角时程数据
    // @param idx 楼层索引
    // @return 层间位移角时程数据序列指针
    QLineSeries *get_fi_idr(std::size_t idx);

    // 获取FilteringIntegral指定楼层位移时程数据
    // @param idx 楼层索引
    // @return 楼层位移时程数据序列指针
    QLineSeries *get_fi_disp(std::size_t idx);

    // 获取FilteringIntegral层间位移角分布数据
    // @param idx 楼层索引
    // @return 层间位移角分布数据序列指针
    QLineSeries *get_fi_all_idr();

    // 获取ModifiedFilteringIntegral指定楼层层间位移角时程数据
    // @param idx 楼层索引
    // @return 层间位移角时程数据序列指针
    QLineSeries *get_mfi_idr(std::size_t idx);

    // 获取ModifiedFilteringIntegral指定楼层位移时程数据
    // @param idx 楼层索引
    // @return 楼层位移时程数据序列指针
    QLineSeries *get_mfi_disp(std::size_t idx);

    // 获取ModifiedFilteringIntegral层间位移角分布数据
    // @return 层间位移角分布数据序列指针
    QLineSeries *get_mfi_all_idr();

private:
    /** 计算对象的数据成员 **/

    // 输入数据成员
    data_structure::Acceleration acc_{};  // 加速度数据
    data_structure::Building building_{}; // 建筑物信息

    // 计算结果对象成员
    gmp_calculation::GmpCalculation gmp_{};   // GMP计算对象
    bool gmp_calculated_ = false;             // GMP计算标志
    size_t cur_idx_ = 0;                      // 当前GMP计算的测点索引
    edp_calculation::FilteringIntegral fi_{}; // 滤波积分计算对象
    bool fi_calculated_ = false;              // 滤波积分计算标志
    edp_calculation::ModifiedFilteringIntegral mfi_{}; // 改进滤波积分计算对象
    bool mfi_calculated_ = false; // 改进滤波积分计算标志
    safty_tagging::BasedOnInterStoryDrift safty_{}; // 安全评估对象
    bool safty_calculated_ = false;                 // 安全评估标志

    // 计算结果的私有函数
    void CalculateGmp(std::size_t idx); // 计算指定测点的GMP
    void CalculateEdpFi();              // 计算滤波积分
    void CalculateEdpMfi();             // 计算改进滤波积分
    void CalculateSafty();              // 计算安全评估

    // 改变输入数据后清除计算结果
    void clear();

    /* 由于计算对象可能过大，因此添加了指针类型，具体是否采用有待考虑
    // GMP计算对象
    std::unique_ptr<gmp_calculation::GmpCalculation> gmp_;
    // 滤波积分计算对象
    std::unique_ptr<edp_calculation::FilteringIntegral>
        edp_fi_;
    // 修正滤波积分计算对象
    std::unique_ptr<edp_calculation::ModifiedFilteringIntegral>
        edp_mfi_;
    // 层间位移角计算结果
    std::unique_ptr<edp_calculation::InterStoryDriftResult>
        edp_result_;
    // 安全评估对象
    std::unique_ptr<safty_tagging::BasedOnInterStoryDrift>
        safty_;
    */

    /** 绘图数据成员 **/

    // 时程数据相关的成员变量
    std::vector<double> time_{};                 // 时间数据
    std::unique_ptr<QLineSeries> acc_series_{};  // 加速度数据
    std::unique_ptr<QLineSeries> vel_series_{};  // 速度数据
    std::unique_ptr<QLineSeries> disp_series_{}; // 位移数据
    // 生成时间横轴的函数
    void get_time_();

    // 反应谱数据相关的成员变量
    std::vector<double> period_{};             // 周期数据
    std::unique_ptr<QLineSeries> sa_series_{}; // Sa数据
    std::unique_ptr<QLineSeries> sv_series_{}; // Sv数据
    std::unique_ptr<QLineSeries> sd_series_{}; // Sd数据
    // 生成周期横轴的函数
    void get_period_();

    // Fourier谱数据相关的成员变量
    std::vector<double> freq_{};                // 频率数据
    std::unique_ptr<QLineSeries> amp_series_{}; // 幅值谱数据
    std::unique_ptr<QLineSeries> pow_series_{}; // 功率谱数据
    // 生成频域横轴的函数
    void get_freq_();

    // FilteringIntegral得到的层间位移角数据相关的成员变量
    std::unique_ptr<QLineSeries> fi_idr_series_{};     // 层间位移角时程
    std::unique_ptr<QLineSeries> fi_disp_series_{};    // 楼层位移时程
    std::unique_ptr<QLineSeries> fi_all_idr_series_{}; // 层间位移角分布
    // ModifiedFilteringIntegral得到的层间位移角数据相关的成员变量
    std::unique_ptr<QLineSeries> mfi_idr_series_{};     // 层间位移角时程
    std::unique_ptr<QLineSeries> mfi_disp_series_{};    // 楼层位移时程
    std::unique_ptr<QLineSeries> mfi_all_idr_series_{}; // 层间位移角分布
    // FilteringIntegral得到的实时层间位移角数据相关的成员变量
    std::unique_ptr<QLineSeries> rt_fi_all_idr_series_{}; // 实时层间位移角分布
};

#endif // CHART_DATA_H