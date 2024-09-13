/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\qrest_ui\main_window_process.cpp
** -----
** File Created: Monday, 26th August 2024 15:49:07
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Thursday, 12th September 2024 11:21:49
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description: 主窗口类的实现，计算流程的实现

// assosiated headers
#include "main_window.h"

// stdc++ headers
#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

// Qt headers
#include <QtCharts/QtCharts>

// project headers
#include "chart_data.h"
#include "numerical_algorithm/math_function.h"
#include "rectangle_widget.h"


// 初始化主页
void QRestMainWindow::InitHomePage()
{
    // 创建加速度时程绘图对象
    QChart *chart_mea_acc = new QChart();
    chart_mea_acc->legend()->setVisible(false);
    ui_->chart_home_mea_acc->setChart(chart_mea_acc);
    ui_->chart_home_mea_acc->setRenderHint(QPainter::Antialiasing);

    // 加速度时间坐标轴
    QValueAxis *axis_time_acc = new QValueAxis();
    axis_time_acc->setTitleText(tr("时间(s)"));
    axis_time_acc->setTickCount(11);
    axis_time_acc->setMinorTickCount(1);
    axis_time_acc->setLabelFormat("%g");
    axis_time_acc->setRange(0,
                            data_interface_->config_.time_count_
                                / data_interface_->config_.frequency_);

    // 加速度坐标轴
    QValueAxis *axis_acc = new QValueAxis();
    axis_acc->setTitleText(tr("加速度(m/s^2)"));
    axis_acc->setTickCount(5);
    axis_acc->setMinorTickCount(1);
    axis_acc->setLabelFormat("%g");

    // 为加速度时程绘图对象添加数据
    QLineSeries *mea_acc = new QLineSeries();
    chart_mea_acc->addSeries(mea_acc);
    chart_mea_acc->addAxis(axis_time_acc, Qt::AlignBottom);
    chart_mea_acc->addAxis(axis_acc, Qt::AlignLeft);
    mea_acc->attachAxis(axis_time_acc);
    mea_acc->attachAxis(axis_acc);

    // 创建位移时程绘图对象
    QChart *chart_mea_disp = new QChart();
    chart_mea_disp->legend()->setVisible(false);
    ui_->chart_home_mea_disp->setChart(chart_mea_disp);
    ui_->chart_home_mea_disp->setRenderHint(QPainter::Antialiasing);

    // 位移时间坐标轴
    QValueAxis *axis_time_disp = new QValueAxis();
    axis_time_disp->setTitleText(tr("时间(s)"));
    axis_time_disp->setTickCount(11);
    axis_time_disp->setMinorTickCount(1);
    axis_time_disp->setLabelFormat("%g");
    axis_time_disp->setRange(0,
                             data_interface_->config_.time_count_
                                 / data_interface_->config_.frequency_);

    // 位移坐标轴
    QValueAxis *axis_disp = new QValueAxis();
    axis_disp->setTitleText(tr("位移(m)"));
    axis_disp->setTickCount(5);
    axis_disp->setMinorTickCount(2);
    axis_disp->setLabelFormat("%g");

    // 为位移时程绘图对象添加数据
    QLineSeries *mea_disp = new QLineSeries();
    chart_mea_disp->addSeries(mea_disp);
    chart_mea_disp->addAxis(axis_time_disp, Qt::AlignBottom);
    chart_mea_disp->addAxis(axis_disp, Qt::AlignLeft);
    mea_disp->attachAxis(axis_time_disp);
    mea_disp->attachAxis(axis_disp);

    // 创建层间位移角绘图对象
    QChart *chart_idr = new QChart();
    chart_idr->setTitle(tr("层间位移角分布").arg(cur_mea_point_ + 1));
    chart_idr->legend()->setVisible(false);
    ui_->chart_home_all_idr->setChart(chart_idr);
    ui_->chart_home_all_idr->setRenderHint(QPainter::Antialiasing);

    // 层间位移角坐标轴
    QValueAxis *axis_idr = new QValueAxis();
    axis_idr->setTitleText(tr("层间位移角"));
    axis_idr->setTickCount(4);
    axis_idr->setMinorTickCount(1);
    axis_idr->setRange(0, 1.2e-4);
    axis_idr->setLabelFormat("%g");

    // 楼层高度坐标轴
    QValueAxis *axis_floor = new QValueAxis();
    axis_floor->setTitleText(tr("楼层高度(m)"));
    axis_floor->setTickCount(11);
    axis_floor->setMinorTickCount(1);
    axis_floor->setRange(data_interface_->building_.get_floor_height().front(),
                         data_interface_->building_.get_floor_height().back());
    axis_floor->setLabelFormat("%g");

    // 为层间位移角绘图对象添加数据
    QLineSeries *idr = new QLineSeries();
    const auto &idr_pnts = chart_data_->get_mfi_all_idr();
    idr->replace(*ChartData::PointsVector2QList(idr_pnts));
    chart_idr->addSeries(idr);
    chart_idr->addAxis(axis_idr, Qt::AlignBottom);
    chart_idr->addAxis(axis_floor, Qt::AlignLeft);
    idr->attachAxis(axis_floor);
    idr->attachAxis(axis_idr);

    // 获取测点数量
    std::size_t mea_num = data_interface_->config_.mea_number_;
    // 为测点选择框添加选项
    for (std::size_t i = 1; i <= mea_num; ++i)
    {
        ui_->cbox_home_mea->addItem(QString::number(i));
    }

    // 已完成主页初始化
    page_initialized_->home_page = true;
}

// 更新Home页面
void QRestMainWindow::UpdateHomePage(std::size_t mea_point)
{
    // 更新主页的内容
    cur_mea_point_ = mea_point;
    // 更新加速度时程图的内容
    auto chart_mea_acc = ui_->chart_home_mea_acc->chart();
    // 更新标题
    chart_mea_acc->setTitle(tr("测点%1的加速度时程").arg(mea_point + 1));
    // 更新数据
    QLineSeries *mea_acc =
        qobject_cast<QLineSeries *>(chart_mea_acc->series().front());
    mea_acc->setName(tr("测点%1的加速度时程").arg(mea_point + 1));
    mea_acc->clear();
    const auto &acc_pnts = chart_data_->get_acceleration(mea_point);
    mea_acc->replace(*ChartData::PointsVector2QList(acc_pnts));
    // 更新坐标轴
    auto max_val = std::abs(*std::max_element(
        acc_pnts.second.begin(), acc_pnts.second.end(), [](double a, double b) {
            return std::abs(a) < std::abs(b);
        }));
    QValueAxis *axis_time_acc =
        qobject_cast<QValueAxis *>(chart_mea_acc->axes(Qt::Vertical).front());
    axis_time_acc->setRange(-max_val, max_val);

    // 更新位移时程图的内容
    auto chart_mea_disp = ui_->chart_home_mea_disp->chart();
    // 更新标题
    chart_mea_disp->setTitle(tr("测点%1的位移时程").arg(mea_point + 1));
    // 更新数据
    QLineSeries *mea_disp =
        qobject_cast<QLineSeries *>(chart_mea_disp->series().front());
    mea_disp->setName(tr("测点%1的位移时程").arg(mea_point + 1));
    mea_disp->clear();
    const auto &disp_pnts = chart_data_->get_displacement(mea_point);
    mea_disp->replace(*ChartData::PointsVector2QList(disp_pnts));
    // 更新坐标轴
    max_val = std::abs(*std::max_element(
        disp_pnts.second.begin(),
        disp_pnts.second.end(),
        [](double a, double b) { return std::abs(a) < std::abs(b); }));
    QValueAxis *axis_time_disp =
        qobject_cast<QValueAxis *>(chart_mea_disp->axes(Qt::Vertical).front());
    axis_time_disp->setRange(-max_val, max_val);
}

// 初始化ACC页面
void QRestMainWindow::InitAccPage()
{
    if (!page_initialized_->acc_tab_time) InitAccTabTime();
    if (!page_initialized_->acc_tab_response) InitAccTabResponse();
    if (!page_initialized_->acc_tab_fourier) InitAccTabFourier();

    // 已完成ACC页面初始化
    page_initialized_->acc_page = true;
}

// 初始化ACC页面下的tab_time
void QRestMainWindow::InitAccTabTime()
{
    // 创建加速度时程绘图对象
    QChart *chart_mea_acc = new QChart();
    chart_mea_acc->legend()->setVisible(false);
    ui_->chart_acc1_a->setChart(chart_mea_acc);
    ui_->chart_acc1_a->setRenderHint(QPainter::Antialiasing);

    // 加速度时间坐标轴
    QValueAxis *axis_time_acc = new QValueAxis();
    axis_time_acc->setTitleText(tr("时间(s)"));
    axis_time_acc->setTickCount(11);
    axis_time_acc->setMinorTickCount(1);
    axis_time_acc->setLabelFormat("%g");
    axis_time_acc->setRange(0,
                            data_interface_->config_.time_count_
                                / data_interface_->config_.frequency_);

    // 加速度坐标轴
    QValueAxis *axis_acc = new QValueAxis();
    axis_acc->setTitleText(tr("加速度(m/s^2)"));
    axis_acc->setTickCount(5);
    axis_acc->setMinorTickCount(1);
    axis_acc->setLabelFormat("%g");

    // 为加速度时程绘图对象添加数据
    QLineSeries *mea_acc = new QLineSeries();
    chart_mea_acc->addSeries(mea_acc);
    chart_mea_acc->addAxis(axis_time_acc, Qt::AlignBottom);
    chart_mea_acc->addAxis(axis_acc, Qt::AlignLeft);
    mea_acc->attachAxis(axis_time_acc);
    mea_acc->attachAxis(axis_acc);

    // 创建速度时程绘图对象
    QChart *chart_mea_vel = new QChart();
    chart_mea_vel->legend()->setVisible(false);
    ui_->chart_acc2_v->setChart(chart_mea_vel);
    ui_->chart_acc2_v->setRenderHint(QPainter::Antialiasing);

    // 速度时间坐标轴
    QValueAxis *axis_time_vel = new QValueAxis();
    axis_time_vel->setTitleText(tr("时间(s)"));
    axis_time_vel->setTickCount(11);
    axis_time_vel->setMinorTickCount(1);
    axis_time_vel->setLabelFormat("%g");
    axis_time_vel->setRange(0,
                            data_interface_->config_.time_count_
                                / data_interface_->config_.frequency_);

    // 速度坐标轴
    QValueAxis *axis_vel = new QValueAxis();
    axis_vel->setTitleText(tr("速度(m/s)"));
    axis_vel->setTickCount(5);
    axis_vel->setMinorTickCount(1);
    axis_vel->setLabelFormat("%g");

    // 为速度时程绘图对象添加数据
    QLineSeries *mea_vel = new QLineSeries();
    chart_mea_vel->addSeries(mea_vel);
    chart_mea_vel->addAxis(axis_time_vel, Qt::AlignBottom);
    chart_mea_vel->addAxis(axis_vel, Qt::AlignLeft);
    mea_vel->attachAxis(axis_time_vel);
    mea_vel->attachAxis(axis_vel);

    // 创建位移时程绘图对象
    QChart *chart_mea_disp = new QChart();
    chart_mea_disp->legend()->setVisible(false);
    ui_->chart_acc3_d->setChart(chart_mea_disp);
    ui_->chart_acc3_d->setRenderHint(QPainter::Antialiasing);

    // 位移时间坐标轴
    QValueAxis *axis_time_disp = new QValueAxis();
    axis_time_disp->setTitleText(tr("时间(s)"));
    axis_time_disp->setTickCount(11);
    axis_time_disp->setMinorTickCount(1);
    axis_time_disp->setLabelFormat("%g");
    axis_time_disp->setRange(0,
                             data_interface_->config_.time_count_
                                 / data_interface_->config_.frequency_);

    // 位移坐标轴
    QValueAxis *axis_disp = new QValueAxis();
    axis_disp->setTitleText(tr("位移(m)"));
    axis_disp->setTickCount(5);
    axis_disp->setMinorTickCount(1);
    axis_disp->setLabelFormat("%g");

    // 为位移时程绘图对象添加数据
    QLineSeries *mea_disp = new QLineSeries();
    chart_mea_disp->addSeries(mea_disp);
    chart_mea_disp->addAxis(axis_time_disp, Qt::AlignBottom);
    chart_mea_disp->addAxis(axis_disp, Qt::AlignLeft);
    mea_disp->attachAxis(axis_time_disp);
    mea_disp->attachAxis(axis_disp);

    // 获取测点数量
    std::size_t mea_num = data_interface_->config_.mea_number_;
    // 为测点选择框添加选项
    for (std::size_t i = 1; i <= mea_num; ++i)
    {
        ui_->cbox_acc_mea->addItem(QString::number(i));
    }

    // 已完成ACC页面下的tab_time初始化
    page_initialized_->acc_tab_time = true;
}

// 更新ACC页面下的tab_time
void QRestMainWindow::UpdateAccTabTime(std::size_t mea_point)
{
    // 更新ACC页面的内容
    cur_mea_point_ = mea_point;
    // 更新加速度时程图的内容
    auto chart_mea_acc = ui_->chart_acc1_a->chart();
    // 更新标题
    chart_mea_acc->setTitle(tr("测点%1的加速度时程").arg(mea_point + 1));
    // 更新数据
    QLineSeries *mea_acc =
        qobject_cast<QLineSeries *>(chart_mea_acc->series().front());
    mea_acc->setName(tr("测点%1的加速度时程").arg(mea_point + 1));
    mea_acc->clear();
    const auto &acc_pnts = chart_data_->get_acceleration(mea_point);
    mea_acc->replace(*ChartData::PointsVector2QList(acc_pnts));
    // 更新坐标轴
    auto max_val = std::abs(*std::max_element(
        acc_pnts.second.begin(), acc_pnts.second.end(), [](double a, double b) {
            return std::abs(a) < std::abs(b);
        }));
    QValueAxis *axis_time_acc =
        qobject_cast<QValueAxis *>(chart_mea_acc->axes(Qt::Vertical).front());
    axis_time_acc->setRange(-max_val, max_val);

    // 更新速度时程图的内容
    auto chart_mea_vel = ui_->chart_acc2_v->chart();
    // 更新标题
    chart_mea_vel->setTitle(tr("测点%1的速度时程").arg(mea_point + 1));
    // 更新数据
    QLineSeries *mea_vel =
        qobject_cast<QLineSeries *>(chart_mea_vel->series().front());
    mea_vel->setName(tr("测点%1的速度时程").arg(mea_point + 1));
    mea_vel->clear();
    const auto &vel_pnts = chart_data_->get_velocity(mea_point);
    mea_vel->replace(*ChartData::PointsVector2QList(vel_pnts));
    // 更新坐标轴
    max_val = std::abs(*std::max_element(
        vel_pnts.second.begin(), vel_pnts.second.end(), [](double a, double b) {
            return std::abs(a) < std::abs(b);
        }));
    QValueAxis *axis_time_vel =
        qobject_cast<QValueAxis *>(chart_mea_vel->axes(Qt::Vertical).front());
    axis_time_vel->setRange(-max_val, max_val);

    // 更新位移时程图的内容
    auto chart_mea_disp = ui_->chart_acc3_d->chart();
    // 更新标题
    chart_mea_disp->setTitle(tr("测点%1的位移时程").arg(mea_point + 1));
    // 更新数据
    QLineSeries *mea_disp =
        qobject_cast<QLineSeries *>(chart_mea_disp->series().front());
    mea_disp->setName(tr("测点%1的位移时程").arg(mea_point + 1));
    mea_disp->clear();
    const auto &disp_pnts = chart_data_->get_displacement(mea_point);
    mea_disp->replace(*ChartData::PointsVector2QList(disp_pnts));
    // 更新坐标轴
    max_val = std::abs(*std::max_element(
        disp_pnts.second.begin(),
        disp_pnts.second.end(),
        [](double a, double b) { return std::abs(a) < std::abs(b); }));
    QValueAxis *axis_time_disp =
        qobject_cast<QValueAxis *>(chart_mea_disp->axes(Qt::Vertical).front());
    axis_time_disp->setRange(-max_val, max_val);
}

// 初始化ACC页面下的tab_response
void QRestMainWindow::InitAccTabResponse()
{
    // 创建反应谱绘图对象
    QChart *chart_gmp = new QChart();
    chart_gmp->legend()->setVisible(false);
    ui_->chart_gmp_response->setChart(chart_gmp);
    ui_->chart_gmp_response->setRenderHint(QPainter::Antialiasing);

    // 周期坐标轴
    QValueAxis *axis_period = new QValueAxis();
    axis_period->setTitleText(tr("周期(s)"));
    axis_period->setTickCount(11);
    axis_period->setMinorTickCount(1);
    axis_period->setLabelFormat("%g");
    // TODO: 设置修正周期范围
    axis_period->setRange(0, 5);

    // 反应谱坐标轴
    QValueAxis *axis_response = new QValueAxis();
    axis_response->setTickCount(5);
    axis_response->setMinorTickCount(1);
    axis_response->setLabelFormat("%g");

    // 为反应谱绘图对象添加数据
    QLineSeries *gmp = new QLineSeries();
    chart_gmp->addSeries(gmp);
    chart_gmp->addAxis(axis_period, Qt::AlignBottom);
    chart_gmp->addAxis(axis_response, Qt::AlignLeft);
    gmp->attachAxis(axis_period);
    gmp->attachAxis(axis_response);

    // 获取测点数量
    std::size_t mea_num = data_interface_->config_.mea_number_;
    // 为测点选择框添加选项
    for (std::size_t i = 1; i <= mea_num; ++i)
    {
        ui_->cbox_gmp_mea->addItem(QString::number(i));
    }

    // 已完成ACC页面下的tab_response初始化
    page_initialized_->acc_tab_response = true;
}

// 更新ACC页面下的tab_response
void QRestMainWindow::UpdateAccTabResponse(std::size_t mea_point,
                                           std::size_t gmp_type)
{
    // 更新反应谱页面的内容
    cur_mea_point_ = mea_point;
    // 更新标题和数据
    QChart *chart_gmp = ui_->chart_gmp_response->chart();
    QLineSeries *gmp = qobject_cast<QLineSeries *>(chart_gmp->series().front());
    gmp->clear();
    QValueAxis *axis_gmp =
        qobject_cast<QValueAxis *>(chart_gmp->axes(Qt::Vertical).front());
    std::pair<std::vector<double>, std::vector<double>> gmp_pnts;
    switch (gmp_type)
    {
        case 0:
            // 加速度谱
            chart_gmp->setTitle(tr("测点%1的加速度反应谱").arg(mea_point + 1));
            axis_gmp->setTitleText(tr("Sa(m/s^2)"));
            gmp->setName(tr("测点%1的加速度反应谱").arg(mea_point + 1));
            gmp_pnts = chart_data_->get_sa(mea_point);
            gmp->replace(*ChartData::PointsVector2QList(gmp_pnts));
            break;
        case 1:
            // 速度谱
            chart_gmp->setTitle(tr("测点%1的速度反应谱").arg(mea_point + 1));
            axis_gmp->setTitleText(tr("Sv(m/s)"));
            gmp->setName(tr("测点%1的速度反应谱").arg(mea_point + 1));
            gmp_pnts = chart_data_->get_sv(mea_point);
            gmp->replace(*ChartData::PointsVector2QList(gmp_pnts));
            break;
        case 2:
            // 位移谱
            chart_gmp->setTitle(tr("测点%1的位移反应谱").arg(mea_point + 1));
            axis_gmp->setTitleText(tr("Sd(m)"));
            gmp->setName(tr("测点%1的位移反应谱").arg(mea_point + 1));
            gmp_pnts = chart_data_->get_sd(mea_point);
            gmp->replace(*ChartData::PointsVector2QList(gmp_pnts));
            break;
        case 3:
            // 拟加速度谱
            break;
        case 4:
            // 拟速度谱
            break;
        default:
            break;
    }
    // 更新坐标轴
    auto max_val = std::abs(
        *std::max_element(gmp_pnts.second.begin(), gmp_pnts.second.end()));
    axis_gmp->setRange(0, max_val);
}

// 初始化ACC页面下的tab_fourier
void QRestMainWindow::InitAccTabFourier()
{
    // 创建fourier谱绘图对象
    QChart *chart_amp = new QChart();
    chart_amp->legend()->setVisible(false);
    ui_->chart_fourier->setChart(chart_amp);
    ui_->chart_fourier->setRenderHint(QPainter::Antialiasing);

    // 幅值谱频率坐标轴
    QValueAxis *axis_amp_freq = new QValueAxis();
    axis_amp_freq->setTitleText(tr("频率(Hz)"));
    axis_amp_freq->setTickCount(11);
    axis_amp_freq->setMinorTickCount(1);
    axis_amp_freq->setLabelFormat("%f");
    axis_amp_freq->setRange(0, data_interface_->config_.frequency_ / 2);

    // 幅值谱坐标轴
    QValueAxis *axis_amp = new QValueAxis();
    axis_amp->setTitleText(tr("幅值"));
    axis_amp->setTickCount(5);
    axis_amp->setMinorTickCount(1);
    axis_amp->setLabelFormat("%g");

    // 为频域谱绘图对象添加数据
    QLineSeries *amp = new QLineSeries();
    chart_amp->addSeries(amp);
    chart_amp->addAxis(axis_amp_freq, Qt::AlignBottom);
    chart_amp->addAxis(axis_amp, Qt::AlignLeft);
    amp->attachAxis(axis_amp_freq);
    amp->attachAxis(axis_amp);

    // 创建功率谱绘图对象
    QChart *chart_pow = new QChart();
    chart_pow->legend()->setVisible(false);
    ui_->chart_power->setChart(chart_pow);
    ui_->chart_power->setRenderHint(QPainter::Antialiasing);

    // 功率谱频率坐标轴
    QValueAxis *axis_pow_freq = new QValueAxis();
    axis_pow_freq->setTitleText(tr("频率(Hz)"));
    axis_pow_freq->setTickCount(11);
    axis_pow_freq->setMinorTickCount(1);
    axis_pow_freq->setLabelFormat("%g");
    axis_pow_freq->setRange(0, data_interface_->config_.frequency_ / 2);

    // 功率谱坐标轴
    QValueAxis *axis_pow = new QValueAxis();
    axis_pow->setTitleText(tr("功率"));
    axis_pow->setTickCount(5);
    axis_pow->setMinorTickCount(1);
    axis_pow->setLabelFormat("%g");

    // 为功率谱绘图对象添加数据
    QLineSeries *pow = new QLineSeries();
    chart_pow->addSeries(pow);
    chart_pow->addAxis(axis_pow_freq, Qt::AlignBottom);
    chart_pow->addAxis(axis_pow, Qt::AlignLeft);
    pow->attachAxis(axis_pow_freq);
    pow->attachAxis(axis_pow);

    // 获取测点数量
    std::size_t mea_num = data_interface_->config_.mea_number_;
    // 为测点选择框添加选项
    for (std::size_t i = 1; i <= mea_num; ++i)
    {
        ui_->cbox_fourier_mea->addItem(QString::number(i));
    }

    // 已完成ACC页面下的tab_fourier初始化
    page_initialized_->acc_tab_fourier = true;
}

// 更新ACC页面下的tab_fourier
void QRestMainWindow::UpdateAccTabFourier(std::size_t mea_point)
{
    // 更新频域谱图的内容
    cur_mea_point_ = mea_point;
    // 更新标题和数据
    QChart *chart_amp = ui_->chart_fourier->chart();
    QLineSeries *amp = qobject_cast<QLineSeries *>(chart_amp->series().front());
    amp->clear();
    QValueAxis *axis_amp_freq =
        qobject_cast<QValueAxis *>(chart_amp->axes(Qt::Vertical).front());
    chart_amp->setTitle(tr("测点%1的幅值谱").arg(mea_point + 1));
    axis_amp_freq->setTitleText(tr("频率(Hz)"));
    amp->setName(tr("测点%1的幅值谱").arg(mea_point + 1));
    const auto &amp_pnts = chart_data_->get_amplitude(mea_point);
    amp->replace(*ChartData::PointsVector2QList(amp_pnts));
    // 更新坐标轴
    auto max_val = std::abs(
        *std::max_element(amp_pnts.second.begin(), amp_pnts.second.end()));
    axis_amp_freq->setRange(0, max_val);

    // 更新功率谱图的内容
    // 更新标题和数据
    QChart *chart_pow = ui_->chart_power->chart();
    QLineSeries *pow = qobject_cast<QLineSeries *>(chart_pow->series().front());
    pow->clear();
    QValueAxis *axis_pow_freq =
        qobject_cast<QValueAxis *>(chart_pow->axes(Qt::Vertical).front());
    chart_pow->setTitle(tr("测点%1的功率谱").arg(mea_point + 1));
    axis_pow_freq->setTitleText(tr("频率(Hz)"));
    pow->setName(tr("测点%1的功率谱").arg(mea_point + 1));
    const auto &pow_pnts = chart_data_->get_power(mea_point);
    pow->replace(*ChartData::PointsVector2QList(pow_pnts));
    // 更新坐标轴
    max_val = std::abs(
        *std::max_element(pow_pnts.second.begin(), pow_pnts.second.end()));
    axis_pow_freq->setRange(0, max_val);
}

// 初始化SHM页面
void QRestMainWindow::InitShmPage()
{
    // 已完成SHM页面初始化
    page_initialized_->shm_page = true;
}

// 初始化EDP页面
void QRestMainWindow::InitEdpPage()
{
    if (!page_initialized_->edp_tab_realtime) InitEdpTabRealtime();
    if (!page_initialized_->edp_tab_algorithm) InitEdpTabAlgorithm();

    // 已完成EDP页面初始化
    page_initialized_->edp_page = true;
}

// 初始化EDP页面下的tab_realtime
void QRestMainWindow::InitEdpTabRealtime()
{
    // 初始化实时

    // 已完成EDP页面下的tab_realtime初始化
    page_initialized_->edp_tab_realtime = true;
}

// 更新EDP页面下的tab_realtime
void QRestMainWindow::UpdateEdpTabRealtime(std::size_t mea_point)
{
    // 更新EDP页面的内容
}

// 初始化EDP页面下的tab_algorithm
void QRestMainWindow::InitEdpTabAlgorithm()
{
    // 创建层间位移角绘图对象
    QChart *chart_idr = new QChart();
    chart_idr->legend()->setVisible(false);
    ui_->chart_edp_al_idr->setChart(chart_idr);
    ui_->chart_edp_al_idr->setRenderHint(QPainter::Antialiasing);

    // 层间位移角时间坐标轴
    QValueAxis *axis_time_idr = new QValueAxis();
    axis_time_idr->setTitleText(tr("时间(s)"));
    axis_time_idr->setTickCount(11);
    axis_time_idr->setMinorTickCount(1);
    axis_time_idr->setRange(0,
                            data_interface_->config_.time_count_
                                / data_interface_->config_.frequency_);
    axis_time_idr->setLabelFormat("%g");

    // 层间位移角坐标轴
    QValueAxis *axis_idr = new QValueAxis();
    axis_idr->setTitleText(tr("层间位移角"));
    axis_idr->setTickCount(5);
    axis_idr->setMinorTickCount(1);
    axis_idr->setLabelFormat("%g");

    // 为层间位移角绘图对象添加数据
    QLineSeries *idr = new QLineSeries();
    chart_idr->addSeries(idr);
    chart_idr->addAxis(axis_time_idr, Qt::AlignBottom);
    chart_idr->addAxis(axis_idr, Qt::AlignLeft);
    idr->attachAxis(axis_time_idr);
    idr->attachAxis(axis_idr);

    // 创建位移时程绘图对象
    QChart *chart_disp = new QChart();
    chart_disp->legend()->setVisible(false);
    ui_->chart_edp_al_disp->setChart(chart_disp);
    ui_->chart_edp_al_disp->setRenderHint(QPainter::Antialiasing);

    // 位移时间坐标轴
    QValueAxis *axis_time_disp = new QValueAxis();
    axis_time_disp->setTitleText(tr("时间(s)"));
    axis_time_disp->setTickCount(11);
    axis_time_disp->setMinorTickCount(1);
    axis_time_disp->setRange(0,
                             data_interface_->config_.time_count_
                                 / data_interface_->config_.frequency_);
    axis_time_disp->setLabelFormat("%g");

    // 位移坐标轴
    QValueAxis *axis_disp = new QValueAxis();
    axis_disp->setTitleText(tr("位移(m)"));
    axis_disp->setTickCount(5);
    axis_disp->setMinorTickCount(1);
    axis_disp->setLabelFormat("%g");

    // 为位移时程绘图对象添加数据
    QLineSeries *disp = new QLineSeries;
    chart_disp->addSeries(disp);
    chart_disp->addAxis(axis_time_disp, Qt::AlignBottom);
    chart_disp->addAxis(axis_disp, Qt::AlignLeft);
    disp->attachAxis(axis_time_disp);
    disp->attachAxis(axis_disp);

    // 创建层间位移角分布绘图对象
    QChart *chart_all_idr = new QChart();
    chart_all_idr->setTitle(tr("层间位移角分布"));
    chart_all_idr->legend()->setVisible(false);
    ui_->chart_edp_al_all_idr->setChart(chart_all_idr);
    ui_->chart_edp_al_all_idr->setRenderHint(QPainter::Antialiasing);

    // 层间位移角坐标轴
    QValueAxis *axis_all_idr = new QValueAxis();
    axis_all_idr->setTitleText(tr("层间位移角"));
    axis_all_idr->setTickCount(5);
    axis_all_idr->setMinorTickCount(1);
    axis_all_idr->setRange(0, 1.2e-4);
    axis_all_idr->setLabelFormat("%g");

    // 楼层高度坐标轴
    QValueAxis *axis_floor = new QValueAxis();
    axis_floor->setTitleText(tr("楼层高度(m)"));
    axis_floor->setTickCount(11);
    axis_floor->setMinorTickCount(1);
    axis_floor->setRange(data_interface_->building_.get_floor_height().front(),
                         data_interface_->building_.get_floor_height().back());
    axis_floor->setLabelFormat("%g");

    // 为层间位移角分布绘图对象添加数据
    QLineSeries *all_idr = new QLineSeries();
    const auto &idr_pnts = chart_data_->get_mfi_all_idr();
    all_idr->replace(*ChartData::PointsVector2QList(idr_pnts));
    chart_all_idr->addSeries(all_idr);
    chart_all_idr->addAxis(axis_all_idr, Qt::AlignBottom);
    chart_all_idr->addAxis(axis_floor, Qt::AlignLeft);
    all_idr->attachAxis(axis_floor);
    all_idr->attachAxis(axis_all_idr);

    // 获取楼层数量
    std::size_t floor_num =
        data_interface_->building_.get_floor_height().size();
    // 为楼层选择框添加选项
    for (std::size_t i = 1; i < floor_num; ++i)
    {
        ui_->cbox_edp_floor->addItem(QString::number(i));
    }

    // 已完成EDP页面下的tab_algorithm初始化
    page_initialized_->edp_tab_algorithm = true;
}

// 更新EDP页面下的tab_algorithm
void QRestMainWindow::UpdateEdpTabAlgorithm(std::size_t floor)
{
    // 更新EDP页面的内容
    cur_floor_ = floor;
    // 更新层间位移角时程图的内容
    auto chart_idr = ui_->chart_edp_al_idr->chart();
    // 更新标题
    chart_idr->setTitle(tr("楼层%1的层间位移角时程").arg(floor + 1));
    // 更新数据
    QLineSeries *idr = qobject_cast<QLineSeries *>(chart_idr->series().front());
    idr->setName(tr("楼层%1的层间位移角时程").arg(floor + 1));
    idr->clear();
    const auto &idr_pnts = chart_data_->get_mfi_idr(floor);
    idr->replace(*ChartData::PointsVector2QList(idr_pnts));
    // 更新坐标轴
    auto max_val = std::abs(*std::max_element(
        idr_pnts.second.begin(), idr_pnts.second.end(), [](double a, double b) {
            return std::abs(a) < std::abs(b);
        }));
    QValueAxis *axis_idr =
        qobject_cast<QValueAxis *>(chart_idr->axes(Qt::Vertical).front());
    axis_idr->setRange(-max_val, max_val);

    // 更新位移时程图的内容
    auto chart_disp = ui_->chart_edp_al_disp->chart();
    // 更新标题
    chart_disp->setTitle(tr("楼层%1的位移时程").arg(floor + 1));
    // 更新数据
    QLineSeries *disp =
        qobject_cast<QLineSeries *>(chart_disp->series().front());
    disp->setName(tr("楼层%1的位移时程").arg(floor + 1));
    disp->clear();
    const auto &disp_pnts = chart_data_->get_mfi_disp(floor);
    disp->replace(*ChartData::PointsVector2QList(disp_pnts));
    // 更新坐标轴
    max_val = std::abs(*std::max_element(
        disp_pnts.second.begin(),
        disp_pnts.second.end(),
        [](double a, double b) { return std::abs(a) < std::abs(b); }));
    QValueAxis *axis_disp =
        qobject_cast<QValueAxis *>(chart_disp->axes(Qt::Vertical).front());
    axis_disp->setRange(-max_val, max_val);
}

// 初始化Result页面
void QRestMainWindow::InitResultPage()
{
    //

    // 已完成Result页面初始化
    page_initialized_->result_page = true;
}