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
#include <vector>

// project headers
#include "gmp_calculation/gmp_calculation.h"
#include "safty_tagging/based_on_inter_story_drift.h"


// 转换point_vector为QList<QPoint>指针的静态函数
std::unique_ptr<QList<QPointF>>
ChartData::PointsVector2QList(const ChartData::points_vector &points)
{
    auto list = std::make_unique<QList<QPointF>>();
    for (std::size_t i = 0; i < points.first.size(); ++i)
    {
        list->emplace_back(points.first[i], points.second[i]);
    }
    return list;
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
    gmp_ = gmp_calculation::GmpCalculation(
        data_interface_->acc_[cur_dir_].col(cur_idx_));
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
    if (safty_[cur_dir_].is_calculated())
    {
        return;
    }
    CalculateEdpMfi();
    safty_[cur_dir_] = safty_tagging::BasedOnInterStoryDrift(
        mfi_[cur_dir_].get_filtering_interp_result());
    safty_[cur_dir_].TagSafty();
}

// 获取加速度数据
ChartData::points_vector ChartData::get_acceleration(std::size_t idx)
{
    // 生成时间横轴
    if (time_.empty())
    {
        get_time_();
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
        get_time_();
    }

    // 获取速度数据
    const auto &vel = gmp_.get_velocity();
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
        get_time_();
    }

    // 获取位移数据
    const auto &disp = gmp_.get_displacement();
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
        get_period_();
    }

    // 获取Sa数据
    const auto &sa = gmp_.AccelerationSpectrum();
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
        get_period_();
    }

    // 获取Sv数据
    const auto &sv = gmp_.VelocitySpectrum();
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
        get_period_();
    }

    // 获取Sd数据
    const auto &sd = gmp_.DisplacementSpectrum();
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
        get_freq_();
    }

    // 获取幅值谱数据
    const auto &amp = gmp_.FourierAmplitudeSpectrum();
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
        get_freq_();
    }

    // 获取功率谱数据
    const auto &pow = gmp_.PowerSpectrum();
    return {freq_, pow};
}

// 获取层间位移角数据
ChartData::points_vector ChartData::get_fi_idr(std::size_t idx)
{
    // 计算滤波积分
    CalculateEdpFi();

    // 生成时间横轴
    if (time_.empty())
    {
        get_time_();
    }

    // 获取层间位移角数据
    const auto &idr =
        fi_[cur_dir_].get_filtering_interp_result().get_inter_story_drift();
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
        get_time_();
    }

    // 获取FilteringIntegral指定楼层位移时程数据
    const auto &disp =
        fi_[cur_dir_].get_filtering_interp_result().get_displacement();
    return {time_, disp.get_col(idx)};
}

// 获取FilteringIntegral层间位移角分布数据
ChartData::points_vector ChartData::get_fi_all_idr()
{
    // 计算滤波积分
    CalculateEdpFi();

    // 获取FilteringIntegral层间位移角分布数据
    safty_[cur_dir_] = safty_tagging::BasedOnInterStoryDrift(
        fi_[cur_dir_].get_filtering_interp_result());
    safty_[cur_dir_].TagSafty();
    const auto &idr = safty_[cur_dir_].get_max_inter_story_drift_result();
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
        get_time_();
    }

    // 获取ModifiedFilteringIntegral指定楼层层间位移角时程数据
    const auto &idr =
        mfi_[cur_dir_].get_filtering_interp_result().get_inter_story_drift();
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
        get_time_();
    }

    // 获取ModifiedFilteringIntegral指定楼层位移时程数据
    const auto &disp =
        mfi_[cur_dir_].get_filtering_interp_result().get_displacement();
    return {time_, disp.get_col(idx)};
}

// 获取ModifiedFilteringIntegral层间位移角分布数据
ChartData::points_vector ChartData::get_mfi_all_idr()
{
    // 计算改进滤波积分
    CalculateEdpMfi();

    // 获取ModifiedFilteringIntegral层间位移角分布数据
    safty_[cur_dir_] = safty_tagging::BasedOnInterStoryDrift(
        mfi_[cur_dir_].get_filtering_interp_result());
    safty_[cur_dir_].TagSafty();
    const auto &idr = safty_[cur_dir_].get_max_inter_story_drift_result();
    std::vector<double> abs_idr(idr.abs_max_idr_.size());
    std::transform(idr.abs_max_idr_.begin(),
                   idr.abs_max_idr_.end(),
                   abs_idr.begin(),
                   [](const double &val) { return std::abs(val); });

    return {abs_idr, data_interface_->building_.get_floor_height()};
}

// 生成横轴时间的函数
void ChartData::get_time_()
{
    time_.resize(data_interface_->config_.time_count_);
    for (std::size_t i = 0; i < time_.size(); ++i)
    {
        time_[i] = 1.0 * i / data_interface_->config_.frequency_;
    }
}

// 生成反应谱横轴的函数
void ChartData::get_period_()
{
    period_.resize(gmp_.AccelerationSpectrum().size());
    for (std::size_t i = 0; i != period_.size(); ++i)
    {
        period_[i] = i * gmp_.get_parameter().response_spectrum_dt_;
    }
}

// 生成Fourier谱横轴的函数
void ChartData::get_freq_()
{
    freq_.resize(gmp_.FourierAmplitudeSpectrum().size());
    for (std::size_t i = 0; i != freq_.size(); ++i)
    {
        freq_[i] = i * gmp_.get_parameter().fourier_spectrum_df_;
    }
}