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

// project headers
#include "gmp_calculation/gmp_calculation.h"
#include "safty_tagging/based_on_inter_story_drift.h"

// 清除计算结果
void ChartData::clear()
{
    // 将计算结果标志位全部置为false
    gmp_calculated_ = false;
    fi_calculated_ = false;
    mfi_calculated_ = false;
    safty_calculated_ = false;

    // 将横轴std::vector置为空
    time_.clear();
    period_.clear();
    freq_.clear();
}

// 计算GMP
void ChartData::CalculateGmp(std::size_t idx)
{
    // 计算对象赋值
    cur_idx_ = idx;
    gmp_ = gmp_calculation::GmpCalculation(acc_.col(cur_idx_));
    gmp_calculated_ = true;
}

// FilteringIntegral计算EDP
void ChartData::CalculateEdpFi()
{
    // 计算对象赋值
    if (fi_calculated_)
    {
        return;
    }
    fi_ = edp_calculation::FilteringIntegral(acc_, building_);
    fi_.CalculateEdp();
    fi_calculated_ = true;
}

// ModifiedFilteringIntegral计算EDP
void ChartData::CalculateEdpMfi()
{
    // 计算对象赋值
    if (mfi_calculated_)
    {
        return;
    }
    mfi_ = edp_calculation::ModifiedFilteringIntegral(acc_, building_);
    mfi_.CalculateEdp();
    mfi_calculated_ = true;
}

// 计算安全评价结果
void ChartData::CalculateSafty()
{
    // 计算对象赋值
    if (safty_calculated_)
    {
        return;
    }
    safty_ = safty_tagging::BasedOnInterStoryDrift(
        mfi_.get_filtering_interp_result());
    safty_.TagSafty();
    safty_calculated_ = true;
}

// 获取加速度数据
QLineSeries *ChartData::get_acceleration(std::size_t idx)
{
    // 生成时间横轴
    if (time_.empty())
    {
        get_time_();
    }

    // 获取加速度数据
    const auto &acc = acc_.col(idx);
    for (std::size_t i = 0; i != time_.size(); ++i)
    {
        acc_series_->append(time_[i], acc[i]);
    }

    return acc_series_.get();
}

// 获取速度数据
QLineSeries *ChartData::get_velocity(std::size_t idx)
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
    for (std::size_t i = 0; i != time_.size(); ++i)
    {
        vel_series_->append(time_[i], vel[i]);
    }

    return vel_series_.get();
}

// 获取位移数据
QLineSeries *ChartData::get_displacement(std::size_t idx)
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
    for (std::size_t i = 0; i != time_.size(); ++i)
    {
        disp_series_->append(time_[i], disp[i]);
    }

    return disp_series_.get();
}

// 获取Sa数据
QLineSeries *ChartData::get_sa(std::size_t idx)
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
    for (std::size_t i = 0; i != period_.size(); ++i)
    {
        sa_series_->append(period_[i], sa[i]);
    }

    return sa_series_.get();
}

// 获取Sv数据
QLineSeries *ChartData::get_sv(std::size_t idx)
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
    for (std::size_t i = 0; i != period_.size(); ++i)
    {
        sv_series_->append(period_[i], sv[i]);
    }

    return sv_series_.get();
}

// 获取Sd数据
QLineSeries *ChartData::get_sd(std::size_t idx)
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
    for (std::size_t i = 0; i != period_.size(); ++i)
    {
        sd_series_->append(period_[i], sd[i]);
    }

    return sd_series_.get();
}

// 获取幅值谱数据
QLineSeries *ChartData::get_amplitude(std::size_t idx)
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
    for (std::size_t i = 0; i != freq_.size(); ++i)
    {
        amp_series_->append(freq_[i], amp[i]);
    }

    return amp_series_.get();
}

// 获取功率谱数据
QLineSeries *ChartData::get_power(std::size_t idx)
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
    for (std::size_t i = 0; i != freq_.size(); ++i)
    {
        pow_series_->append(freq_[i], pow[i]);
    }

    return pow_series_.get();
}

// 获取层间位移角数据
QLineSeries *ChartData::get_fi_idr(std::size_t idx)
{
    // 计算滤波积分
    if (!fi_calculated_)
    {
        CalculateEdpFi();
    }

    // 生成时间横轴
    if (time_.empty())
    {
        get_time_();
    }

    // 获取层间位移角数据
    const auto &idr = fi_.get_filtering_interp_result().get_inter_story_drift();
    for (std::size_t i = 0; i != time_.size(); ++i)
    {
        fi_idr_series_->append(time_[i], idr.get_col(idx)[i]);
    }

    return fi_idr_series_.get();
}

// 获取FilteringIntegral指定楼层位移时程数据
QLineSeries *ChartData::get_fi_disp(std::size_t idx)
{
    // 计算滤波积分
    if (!fi_calculated_)
    {
        CalculateEdpFi();
    }

    // 生成时间横轴
    if (time_.empty())
    {
        get_time_();
    }

    // 获取FilteringIntegral指定楼层位移时程数据
    const auto &disp = fi_.get_filtering_interp_result().get_displacement();
    for (std::size_t i = 0; i != time_.size(); ++i)
    {
        fi_disp_series_->append(time_[i], disp.get_col(idx)[i]);
    }

    return fi_disp_series_.get();
}

// 获取FilteringIntegral层间位移角分布数据
QLineSeries *ChartData::get_fi_all_idr()
{
    // 计算滤波积分
    if (!fi_calculated_)
    {
        CalculateEdpFi();
    }

    // 获取FilteringIntegral层间位移角分布数据
    safty_ = safty_tagging::BasedOnInterStoryDrift(
        fi_.get_filtering_interp_result());
    safty_.TagSafty();
    const auto &idr = safty_.get_max_inter_story_drift_result();
    for (std::size_t i = 0; i != idr.abs_max_inter_story_drift_.size(); ++i)
    {
        fi_all_idr_series_->append(idr.abs_max_inter_story_drift_[i].first,
                                   building_.get_floor_height()[i + 1]);
    }

    return fi_all_idr_series_.get();
}

// 获取ModifiedFilteringIntegral指定楼层层间位移角时程数据
QLineSeries *ChartData::get_mfi_idr(std::size_t idx)
{
    // 计算改进滤波积分
    if (!mfi_calculated_)
    {
        CalculateEdpMfi();
    }

    // 生成时间横轴
    if (time_.empty())
    {
        get_time_();
    }

    // 获取ModifiedFilteringIntegral指定楼层层间位移角时程数据
    const auto &idr =
        mfi_.get_filtering_interp_result().get_inter_story_drift();
    for (std::size_t i = 0; i != time_.size(); ++i)
    {
        mfi_idr_series_->append(time_[i], idr.get_col(idx)[i]);
    }

    return mfi_idr_series_.get();
}

// 获取ModifiedFilteringIntegral指定楼层位移时程数据
QLineSeries *ChartData::get_mfi_disp(std::size_t idx)
{
    // 计算改进滤波积分
    if (!mfi_calculated_)
    {
        CalculateEdpMfi();
    }

    // 生成时间横轴
    if (time_.empty())
    {
        get_time_();
    }

    // 获取ModifiedFilteringIntegral指定楼层位移时程数据
    const auto &disp = mfi_.get_filtering_interp_result().get_displacement();
    for (std::size_t i = 0; i != time_.size(); ++i)
    {
        mfi_disp_series_->append(time_[i], disp.get_col(idx)[i]);
    }

    return mfi_disp_series_.get();
}

// 获取ModifiedFilteringIntegral层间位移角分布数据
QLineSeries *ChartData::get_mfi_all_idr()
{
    // 计算改进滤波积分
    if (!mfi_calculated_)
    {
        CalculateEdpMfi();
    }

    // 获取ModifiedFilteringIntegral层间位移角分布数据
    safty_ = safty_tagging::BasedOnInterStoryDrift(
        mfi_.get_filtering_interp_result());
    safty_.TagSafty();
    const auto &idr = safty_.get_max_inter_story_drift_result();
    for (std::size_t i = 0; i != idr.abs_max_inter_story_drift_.size(); ++i)
    {
        mfi_all_idr_series_->append(idr.abs_max_inter_story_drift_[i].first,
                                    building_.get_floor_height()[i + 1]);
    }

    return mfi_all_idr_series_.get();
}

// 生成横轴时间的函数
void ChartData::get_time_()
{
    time_.resize(acc_.col(0).size());
    for (std::size_t i = 0; i < time_.size(); ++i)
    {
        time_[i] = i * acc_.get_time_step();
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