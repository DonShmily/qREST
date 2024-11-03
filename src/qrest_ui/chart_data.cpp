/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\qrest_ui\chart_data.cpp
** -----
** File Created: Monday, 26th August 2024 09:35:18
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Tuesday, 27th August 2024 11:45:04
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description: 图表数据类的实现

// associated header
#include "chart_data.h"

// stdc++ headers
#include <cstddef>
#include <memory>
#include <vector>

// project headers
#include "gmp_calculation/gmp_calculation.h"
#include "parameters_identification/simple_parameters_identification.h"
#include "safty_tagging/based_on_inter_story_drift.h"


// 转换point_vector为QList<QPoint>指针的静态函数
std::unique_ptr<QList<QPointF>>
ChartData::PointsVector2PointList(const ChartData::points_vector &points)
{
    auto list = std::make_unique<QList<QPointF>>();
    for (std::size_t i = 0; i < points.first.size(); ++i)
    {
        list->emplace_back(points.first[i], points.second[i]);
    }
    return list;
}

// 转换point_vector为std::pair<QList<qreal>, QList<qreal>>指针的静态函数
std::unique_ptr<std::pair<QList<qreal>, QList<qreal>>>
ChartData::PointsVector2DoubleList(const ChartData::points_vector &points)
{
    auto pnt_list = std::make_unique<std::pair<QList<qreal>, QList<qreal>>>();
    for (std::size_t i = 0; i < points.first.size(); ++i)
    {
        pnt_list->first.push_back(points.first[i]);
        pnt_list->second.push_back(points.second[i]);
    }
    return pnt_list;
}

// 清除计算结果
void ChartData::clear()
{
    // 将计算结果标志位全部置为false
    gmp_calculated_ = false;
    time_.clear();
    period_.clear();
    freq_.clear();
}

// 计算GMP
void ChartData::CalculateGmp(std::size_t idx)
{
    // 计算对象赋值
    cur_idx_ = idx;
    gmp_[cur_dir_] = gmp_calculation::GmpCalculation(
        data_interface_->acc_[cur_dir_].col(cur_idx_));
}

// 参数识别
void ChartData::CalculateSpi()
{
    // 计算对象赋值
    if (spi_[cur_dir_].is_calculated())
    {
        return;
    }
    spi_[cur_dir_] = parameters_identification::SimpleParametersIdentification(
        data_interface_->acc_[cur_dir_], data_interface_->building_);
    spi_[cur_dir_].Identify();
}

// FilteringIntegral计算EDP
void ChartData::CalculateEdpFi()
{
    // 计算对象赋值
    if (fi_[cur_dir_].is_calculated())
    {
        return;
    }
    fi_[cur_dir_] = edp_calculation::FilteringIntegral(
        data_interface_->acc_[cur_dir_], data_interface_->building_);
    fi_[cur_dir_].CalculateEdp();
}

// ModifiedFilteringIntegral计算EDP
void ChartData::CalculateEdpMfi()
{
    // 计算对象赋值
    if (mfi_[cur_dir_].is_calculated())
    {
        return;
    }
    mfi_[cur_dir_] = edp_calculation::ModifiedFilteringIntegral(
        data_interface_->acc_[cur_dir_], data_interface_->building_);
    mfi_[cur_dir_].CalculateEdp();
}

// 计算安全评价结果
void ChartData::CalculateSafty()
{
    // 计算对象赋值
    if (safty_idr_[cur_dir_].is_calculated())
    {
        return;
    }
    CalculateEdpMfi();
    safty_idr_[cur_dir_] =
        safty_tagging::BasedOnInterStoryDrift(mfi_[cur_dir_].get_result_ptr());
    safty_idr_[cur_dir_].TagSafty();
}

// 获取加速度数据
ChartData::points_vector ChartData::get_acceleration(std::size_t idx)
{
    // 生成时间横轴
    if (time_.empty())
    {
        get_time();
    }

    // 获取加速度数据
    const auto &acc = data_interface_->acc_[cur_dir_].col(idx);
    return {time_, acc};
}

// 获取速度数据
ChartData::points_vector ChartData::get_velocity(std::size_t idx)
{
    // 计算GMP
    if (!gmp_calculated_ || cur_idx_ != idx)
    {
        CalculateGmp(idx);
    }

    // 生成时间横轴
    if (time_.empty())
    {
        get_time();
    }

    // 获取速度数据
    const auto &vel = gmp_[cur_dir_].get_velocity();
    return {time_, vel};
}

// 获取位移数据
ChartData::points_vector ChartData::get_displacement(std::size_t idx)
{
    // 计算GMP
    if (!gmp_calculated_ || cur_idx_ != idx)
    {
        CalculateGmp(idx);
    }

    // 生成时间横轴
    if (time_.empty())
    {
        get_time();
    }

    // 获取位移数据
    const auto &disp = gmp_[cur_dir_].get_displacement();
    return {time_, disp};
}

// 获取Sa数据
ChartData::points_vector ChartData::get_sa(std::size_t idx)
{
    // 计算GMP
    if (!gmp_calculated_ || cur_idx_ != idx)
    {
        CalculateGmp(idx);
    }

    // 生成周期横轴
    if (period_.empty())
    {
        get_period();
    }

    // 获取Sa数据
    const auto &sa = gmp_[cur_dir_].AccelerationSpectrum();
    return {period_, sa};
}

// 获取Sv数据
ChartData::points_vector ChartData::get_sv(std::size_t idx)
{
    // 计算GMP
    if (!gmp_calculated_ || cur_idx_ != idx)
    {
        CalculateGmp(idx);
    }

    // 生成周期横轴
    if (period_.empty())
    {
        get_period();
    }

    // 获取Sv数据
    const auto &sv = gmp_[cur_dir_].VelocitySpectrum();
    return {period_, sv};
}

// 获取Sd数据
ChartData::points_vector ChartData::get_sd(std::size_t idx)
{
    // 计算GMP
    if (!gmp_calculated_ || cur_idx_ != idx)
    {
        CalculateGmp(idx);
    }

    // 生成周期横轴
    if (period_.empty())
    {
        get_period();
    }

    // 获取Sd数据
    const auto &sd = gmp_[cur_dir_].DisplacementSpectrum();
    return {period_, sd};
}

// 获取幅值谱数据
ChartData::points_vector ChartData::get_amplitude(std::size_t idx)
{
    // 计算GMP
    if (!gmp_calculated_ || cur_idx_ != idx)
    {
        CalculateGmp(idx);
    }

    // 生成频率横轴
    if (freq_.empty())
    {
        get_freqency();
    }

    // 获取幅值谱数据
    const auto &amp = gmp_[cur_dir_].FourierAmplitudeSpectrum();
    return {freq_, amp};
}

// 获取功率谱数据
ChartData::points_vector ChartData::get_power(std::size_t idx)
{
    // 计算GMP
    if (!gmp_calculated_ || cur_idx_ != idx)
    {
        CalculateGmp(idx);
    }

    // 生成频率横轴
    if (freq_.empty())
    {
        get_freqency();
    }

    // 获取功率谱数据
    const auto &pow = gmp_[cur_dir_].PowerSpectrum();
    return {freq_, pow};
}

// 获取识别的频率数据
ChartData::points_vector ChartData::get_pi_frequency()
{
    // 计算参数识别
    CalculateSpi();

    // 获取识别的频率数据
    const auto &freq = spi_[cur_dir_].identify_parameters().get_frequency();
    return {std::vector<double>(freq.size(), 0), freq};
}

// 获得识别的周期数据
ChartData::points_vector ChartData::get_pi_period()
{
    // 计算参数识别
    CalculateSpi();

    // 获取识别的周期数据
    const auto &period = spi_[cur_dir_].identify_parameters().get_period();
    return {std::vector<double>(period.size(), 0), period};
}

// 获取识别的阻尼比数据
ChartData::points_vector ChartData::get_pi_damping_ratio()
{
    // 计算参数识别
    CalculateSpi();

    // 获取识别的阻尼比数据
    const auto &damp = spi_[cur_dir_].identify_parameters().get_damping_ratio();
    return {std::vector<double>(damp.size(), 0), damp};
}

// 获取识别的模态数据
std::vector<ChartData::points_vector> ChartData::get_pi_mode()
{
    // 计算参数识别
    CalculateSpi();

    // 获取识别的模态数据
    const auto &modal = spi_[cur_dir_].identify_parameters().get_mode_shape();
    std::vector<ChartData::points_vector> modes;
    const auto &mea_height = data_interface_->building_.get_measuren_height();
    for (std::size_t i = 0; i < modal.size(); ++i)
    {
        modes.push_back(
            {modal[i],
             std::vector<double>(mea_height.begin() + 1, mea_height.end())});
    }
    return modes;
}

// 获取层间位移角数据
ChartData::points_vector ChartData::get_fi_idr(std::size_t idx)
{
    // 计算滤波积分
    CalculateEdpFi();

    // 生成时间横轴
    if (time_.empty())
    {
        get_time();
    }

    // 获取层间位移角数据
    const auto &idr = fi_[cur_dir_].get_result().get_inter_story_drift();
    return {time_, idr.get_col(idx)};
}

// 获取FilteringIntegral指定楼层位移时程数据
ChartData::points_vector ChartData::get_fi_disp(std::size_t idx)
{
    // 计算滤波积分
    CalculateEdpFi();

    // 生成时间横轴
    if (time_.empty())
    {
        get_time();
    }

    // 获取FilteringIntegral指定楼层位移时程数据
    const auto &disp = fi_[cur_dir_].get_result().get_displacement();
    return {time_, disp.get_col(idx)};
}

// 获取FilteringIntegral层间位移角分布数据
ChartData::points_vector ChartData::get_fi_all_idr()
{
    // 计算滤波积分
    CalculateEdpFi();

    // 获取FilteringIntegral层间位移角分布数据
    safty_idr_[cur_dir_] =
        safty_tagging::BasedOnInterStoryDrift(fi_[cur_dir_].get_result_ptr());
    safty_idr_[cur_dir_].TagSafty();
    const auto &idr = safty_idr_[cur_dir_].get_max_inter_story_drift_result();
    std::vector<double> abs_idr(idr.abs_max_idr_.size());
    std::transform(idr.abs_max_idr_.begin(),
                   idr.abs_max_idr_.end(),
                   abs_idr.begin(),
                   [](const double &val) { return std::abs(val); });

    return {idr.abs_max_idr_, data_interface_->building_.get_floor_height()};
}

// 获取ModifiedFilteringIntegral指定楼层层间位移角时程数据
ChartData::points_vector ChartData::get_mfi_idr(std::size_t idx)
{
    // 计算改进滤波积分
    CalculateEdpMfi();

    // 生成时间横轴
    if (time_.empty())
    {
        get_time();
    }

    // 获取ModifiedFilteringIntegral指定楼层层间位移角时程数据
    const auto &idr = mfi_[cur_dir_].get_result().get_inter_story_drift();
    return {time_, idr.get_col(idx)};
}

// 获取ModifiedFilteringIntegral指定楼层位移时程数据
ChartData::points_vector ChartData::get_mfi_disp(std::size_t idx)
{
    // 计算改进滤波积分
    CalculateEdpMfi();

    // 生成时间横轴
    if (time_.empty())
    {
        get_time();
    }

    // 获取ModifiedFilteringIntegral指定楼层位移时程数据
    const auto &disp = mfi_[cur_dir_].get_result().get_displacement();
    return {time_, disp.get_col(idx)};
}

// 获取ModifiedFilteringIntegral指定楼层加速度时程数据
ChartData::points_vector ChartData::get_mfi_acc(std::size_t idx)
{
    // 计算改进滤波积分
    CalculateEdpMfi();

    // 生成时间横轴
    if (time_.empty())
    {
        get_time();
    }

    // 获取ModifiedFilteringIntegral指定楼层加速度时程数据
    const auto &acc = mfi_[cur_dir_].get_result().get_acceleration();
    return {time_, acc.get_col(idx)};
}

// 获取ModifiedFilteringIntegral层间位移角分布数据
ChartData::points_vector ChartData::get_mfi_all_idr()
{
    // 计算改进滤波积分
    CalculateEdpMfi();

    // 获取ModifiedFilteringIntegral层间位移角分布数据
    safty_idr_[cur_dir_] =
        safty_tagging::BasedOnInterStoryDrift(mfi_[cur_dir_].get_result_ptr());
    safty_idr_[cur_dir_].TagSafty();
    const auto &idr = safty_idr_[cur_dir_].get_max_inter_story_drift_result();
    std::vector<double> abs_idr(idr.abs_max_idr_.size());
    std::transform(idr.abs_max_idr_.begin(),
                   idr.abs_max_idr_.end(),
                   abs_idr.begin(),
                   [](const double &val) { return std::abs(val); });

    // 以现有值生成阶梯图数据
    std::vector<double> new_abs_idr;
    std::vector<double> floor;
    for (std::size_t i = 0; i != 2 * abs_idr.size(); ++i)
    {
        new_abs_idr.push_back(abs_idr[i / 2]);
        if (i)
        {
            floor.push_back(
                data_interface_->building_.get_floor_height()[(i + 1) / 2]);
        }
        else
        {
            floor.push_back(data_interface_->building_.get_floor_height()[0]);
        }
    }

    return {new_abs_idr, floor};
}

// 获取层间位移角评估限值数据
std::vector<ChartData::points_vector> ChartData::get_idr_safty_limit()
{
    // 计算安全评价
    CalculateSafty();

    // 获取评估限值数据
    const auto &height = data_interface_->building_.get_floor_height();
    std::vector<double> bottom_top{height.front(), height.back()};
    std::vector<double> limit = safty_idr_[cur_dir_].get_safty_tagging_limit();

    std::vector<ChartData::points_vector> limit_data;
    for (std::size_t i = 0; i < limit.size(); ++i)
    {
        limit_data.push_back({std::vector<double>(2, limit[i]), bottom_top});
    }
    return limit_data;
}

// TODO:获取楼面峰值加速度评估限值数据
std::vector<ChartData::points_vector> ChartData::get_acc_safty_limit()
{
    // 计算安全评价
    CalculateSafty();

    // 获取评估限值数据
    const auto &height = data_interface_->building_.get_floor_height();
    std::vector<double> bottom_top{height.front(), height.back()};
    std::vector<double> limit = safty_acc_[cur_dir_].get_safty_tagging_limit();

    std::vector<ChartData::points_vector> limit_data;
    for (std::size_t i = 0; i < limit.size(); ++i)
    {
        limit_data.push_back({std::vector<double>(2, limit[i]), bottom_top});
    }
    return limit_data;
}

// 获取最大楼面加速度分布数据
ChartData::points_vector ChartData::get_max_acc()
{
    // 计算安全评价
    CalculateEdpMfi();

    // 获取最大楼面加速度数据
    safty_acc_[cur_dir_] =
        safty_tagging::BasedOnAcceleration(mfi_[cur_dir_].get_result_ptr());
    safty_acc_[cur_dir_].TagSafty();
    const auto &acc = safty_acc_[cur_dir_].get_max_acc_result();
    std::vector<double> abs_acc(acc.abs_max_acc_.size());
    std::transform(acc.abs_max_acc_.begin(),
                   acc.abs_max_acc_.end(),
                   abs_acc.begin(),
                   [](const double &val) { return std::abs(val); });

    // 以现有值生成阶梯图数据
    std::vector<double> new_abs_acc;
    std::vector<double> floor;
    for (std::size_t i = 0; i != 2 * abs_acc.size() - 1; ++i)
    {
        new_abs_acc.push_back(abs_acc[i / 2]);
        if (i)
        {
            floor.push_back(
                data_interface_->building_.get_floor_height()[(i + 1) / 2]);
        }
        else
        {
            floor.push_back(data_interface_->building_.get_floor_height()[0]);
        }
    }
    new_abs_acc.pop_back();
    floor.pop_back();

    return {new_abs_acc, floor};
}

// 获取安全评价结果
int ChartData::get_safty_result()
{
    // 计算安全评价
    CalculateSafty();

    return safty_idr_[cur_dir_].get_tagging_result();
}

// 生成横轴时间的函数
void ChartData::get_time()
{
    time_.resize(data_interface_->config_.time_count_);
    for (std::size_t i = 0; i < time_.size(); ++i)
    {
        time_[i] = 1.0 * i / data_interface_->config_.frequency_;
    }
}

// 生成反应谱横轴的函数
void ChartData::get_period()
{
    period_.resize(gmp_[cur_dir_].AccelerationSpectrum().size());
    for (std::size_t i = 0; i != period_.size(); ++i)
    {
        period_[i] = i * gmp_[cur_dir_].get_parameter().response_spectrum_dt_;
    }
}

// 生成Fourier谱横轴的函数
void ChartData::get_freqency()
{
    freq_.resize(gmp_[cur_dir_].FourierAmplitudeSpectrum().size());
    for (std::size_t i = 0; i != freq_.size(); ++i)
    {
        freq_[i] = i * gmp_[cur_dir_].get_parameter().fourier_spectrum_df_;
    }
}