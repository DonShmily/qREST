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
** Last Modified: Monday, 26th August 2024 15:49:26
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description: 主窗口类的实现，计算流程的实现

// assosiated headers
#include "main_window.h"
#include "qrest_ui/chart_data.h"
#include "QtCharts/qchart.h"


// stdc++ headers
#include <cstddef>

// Qt headers
#include <QtCharts/QtCharts>

// 初始化主页
void QRestMainWindow::InitHomePage()
{
    // 获取测点数量
    // TODO: 修正一些参数的读取
    std::size_t mea_num = data_interface->config_.mea_number_;
    if (ui->cbox_home_mea->count() == 0)
    {
        ui->cbox_home_mea->blockSignals(true);
        for (std::size_t i = 1; i <= mea_num; ++i)
        {
            ui->cbox_home_mea->addItem(QString::number(i));
        }
        ui->cbox_home_mea->blockSignals(false);
    }

    // 创建加速度时程绘图对象
    QChart *chart_mea_acc = new QChart();
    chart_mea_acc->setTitle(tr("测点%1的加速度时程").arg(cur_mea_point + 1));
    chart_mea_acc->legend()->setVisible(false);
    ui->chart_home_mea_acc->setChart(chart_mea_acc);
    ui->chart_home_mea_acc->setRenderHint(QPainter::Antialiasing);

    // 加速度时间坐标轴
    QValueAxis *axis_time_acc = new QValueAxis();
    axis_time_acc->setTitleText(tr("时间(s)"));
    axis_time_acc->setTickCount(11);
    axis_time_acc->setMinorTickCount(2);
    axis_time_acc->setRange(0,
                            data_interface->config_.time_count_
                                / data_interface->config_.frequency_);
    axis_time_acc->setLabelFormat("%f");

    // 加速度坐标轴
    QValueAxis *axis_acc = new QValueAxis();
    axis_acc->setTitleText(tr("加速度(m/s^2)"));
    axis_acc->setTickCount(5);
    axis_acc->setMinorTickCount(2);
    axis_acc->setRange(-0.04, 0.04);
    axis_acc->setLabelFormat("%.2e");

    // 为加速度时程绘图对象添加数据
    QLineSeries *mea_acc = chart_data->get_acceleration(cur_mea_point);
    mea_acc->setName(tr("测点%1的加速度时程").arg(cur_mea_point + 1));
    chart_mea_acc->addSeries(mea_acc);
    // chart_mea_acc->createDefaultAxes();
    chart_mea_acc->addAxis(axis_time_acc, Qt::AlignBottom);
    chart_mea_acc->addAxis(axis_acc, Qt::AlignLeft);
    mea_acc->attachAxis(axis_time_acc);
    mea_acc->attachAxis(axis_acc);

    // 创建位移时程绘图对象
    QChart *chart_mea_disp = new QChart();
    chart_mea_disp->setTitle(tr("测点%1的位移时程").arg(cur_mea_point + 1));
    chart_mea_disp->legend()->setVisible(false);
    ui->chart_home_mea_disp->setChart(chart_mea_disp);
    ui->chart_home_mea_disp->setRenderHint(QPainter::Antialiasing);

    // 位移时间坐标轴
    QValueAxis *axis_time_disp = new QValueAxis();
    axis_time_disp->setTitleText(tr("时间(s)"));
    axis_time_disp->setTickCount(11);
    axis_time_disp->setMinorTickCount(2);
    axis_time_disp->setRange(0,
                             data_interface->config_.time_count_
                                 / data_interface->config_.frequency_);
    axis_time_disp->setLabelFormat("%f");

    // 位移坐标轴
    QValueAxis *axis_disp = new QValueAxis();
    axis_disp->setTitleText(tr("位移(m)"));
    axis_disp->setTickCount(5);
    axis_disp->setMinorTickCount(2);
    axis_disp->setRange(-0.1, 0.1);
    axis_disp->setLabelFormat("%.2e");

    // 为位移时程绘图对象添加数据
    QLineSeries *mea_disp = chart_data->get_displacement(cur_mea_point);
    mea_disp->setName(tr("测点%1的位移时程").arg(cur_mea_point + 1));
    chart_mea_disp->addSeries(mea_disp);
    // chart_mea_disp->createDefaultAxes();
    chart_mea_disp->addAxis(axis_time_disp, Qt::AlignBottom);
    chart_mea_disp->addAxis(axis_disp, Qt::AlignLeft);
    mea_disp->attachAxis(axis_time_disp);
    mea_disp->attachAxis(axis_disp);

    // 创建层间位移角绘图对象
    QChart *chart_idr = new QChart();
    chart_idr->setTitle(tr("层间位移角分布").arg(cur_mea_point + 1));
    chart_idr->legend()->setVisible(false);
    ui->chart_home_all_idr->setChart(chart_idr);
    ui->chart_home_all_idr->setRenderHint(QPainter::Antialiasing);

    // 层间位移角坐标轴
    QValueAxis *axis_idr = new QValueAxis();
    axis_idr->setTitleText(tr("层间位移角"));
    axis_idr->setTickCount(5);
    axis_idr->setMinorTickCount(2);
    axis_idr->setRange(0, 1.2e-4);
    axis_idr->setLabelFormat("%.2e");

    // 楼层高度坐标轴
    QValueAxis *axis_floor = new QValueAxis();
    axis_floor->setTitleText(tr("楼层高度(m)"));
    axis_floor->setTickCount(11);
    axis_floor->setMinorTickCount(2);
    axis_floor->setRange(data_interface->building_.get_floor_height().front(),
                         data_interface->building_.get_floor_height().back());
    axis_floor->setLabelFormat("%.1f");

    // 为层间位移角绘图对象添加数据
    QLineSeries *idr = chart_data->get_mfi_all_idr();
    chart_idr->addSeries(idr);
    // chart_idr->createDefaultAxes();
    chart_idr->addAxis(axis_idr, Qt::AlignBottom);
    chart_idr->addAxis(axis_floor, Qt::AlignLeft);
    idr->attachAxis(axis_floor);
    idr->attachAxis(axis_idr);

    ui->cbox_home_mea->setCurrentIndex(cur_mea_point);
}

// 更新Home页面加速度和位移的数据
void QRestMainWindow::UpdateHomePage(std::size_t mea_point)
{
    cur_mea_point = mea_point;
    InitHomePage();
}

// 初始化ACC页面
void QRestMainWindow::InitAccPage()
{
    // 获取测点数量
    std::size_t mea_num = data_interface->config_.mea_number_;
    if (ui->cbox_acc_mea->count() == 0)
    {
        ui->cbox_acc_mea->blockSignals(true);
        for (std::size_t i = 1; i <= mea_num; ++i)
        {
            ui->cbox_acc_mea->addItem(QString::number(i));
        }
        ui->cbox_acc_mea->blockSignals(false);
    }

    // 创建加速度时程绘图对象
    QChart *chart_mea_acc = new QChart();
    chart_mea_acc->setTitle(tr("测点%1的加速度时程").arg(cur_mea_point + 1));
    chart_mea_acc->legend()->setVisible(false);
    ui->chart_acc1_a->setChart(chart_mea_acc);
    ui->chart_acc1_a->setRenderHint(QPainter::Antialiasing);

    // 加速度时间坐标轴
    QValueAxis *axis_time_acc = new QValueAxis();
    axis_time_acc->setTitleText(tr("时间(s)"));
    axis_time_acc->setTickCount(11);
    axis_time_acc->setMinorTickCount(2);
    axis_time_acc->setRange(0,
                            data_interface->config_.time_count_
                                / data_interface->config_.frequency_);
    axis_time_acc->setLabelFormat("%f");

    // 加速度坐标轴
    QValueAxis *axis_acc = new QValueAxis();
    axis_acc->setTitleText(tr("加速度(m/s^2)"));
    axis_acc->setTickCount(5);
    axis_acc->setMinorTickCount(2);
    axis_acc->setRange(-0.04, 0.04);
    axis_acc->setLabelFormat("%.2e");

    // 为加速度时程绘图对象添加数据
    QLineSeries *mea_acc = chart_data->get_acceleration(cur_mea_point);
    mea_acc->setName(tr("测点%1的加速度时程").arg(cur_mea_point + 1));
    chart_mea_acc->addSeries(mea_acc);
    // chart_mea_acc->createDefaultAxes();
    chart_mea_acc->addAxis(axis_time_acc, Qt::AlignBottom);
    chart_mea_acc->addAxis(axis_acc, Qt::AlignLeft);
    mea_acc->attachAxis(axis_time_acc);
    mea_acc->attachAxis(axis_acc);

    // 创建速度时程绘图对象
    QChart *chart_mea_vel = new QChart();
    chart_mea_vel->setTitle(tr("测点%1的速度时程").arg(cur_mea_point + 1));
    chart_mea_vel->legend()->setVisible(false);
    ui->chart_acc2_v->setChart(chart_mea_vel);
    ui->chart_acc2_v->setRenderHint(QPainter::Antialiasing);

    // 速度时间坐标轴
    QValueAxis *axis_time_vel = new QValueAxis();
    axis_time_vel->setTitleText(tr("时间(s)"));
    axis_time_vel->setTickCount(11);
    axis_time_vel->setMinorTickCount(2);
    axis_time_vel->setRange(0,
                            data_interface->config_.time_count_
                                / data_interface->config_.frequency_);
    axis_time_vel->setLabelFormat("%f");

    // 速度坐标轴
    QValueAxis *axis_vel = new QValueAxis();
    axis_vel->setTitleText(tr("速度(m/s)"));
    axis_vel->setTickCount(5);
    axis_vel->setMinorTickCount(2);
    axis_vel->setRange(-0.08, 0.08);
    axis_vel->setLabelFormat("%.2e");

    // 为速度时程绘图对象添加数据
    QLineSeries *mea_vel = chart_data->get_velocity(cur_mea_point);
    mea_vel->setName(tr("测点%1的速度时程").arg(cur_mea_point + 1));
    chart_mea_vel->addSeries(mea_vel);
    // chart_mea_vel->createDefaultAxes();
    chart_mea_vel->addAxis(axis_time_vel, Qt::AlignBottom);
    chart_mea_vel->addAxis(axis_vel, Qt::AlignLeft);
    mea_vel->attachAxis(axis_time_vel);
    mea_vel->attachAxis(axis_vel);

    // 创建位移时程绘图对象
    QChart *chart_mea_disp = new QChart();
    chart_mea_disp->setTitle(tr("测点%1的位移时程").arg(cur_mea_point + 1));
    chart_mea_disp->legend()->setVisible(false);
    ui->chart_acc3_d->setChart(chart_mea_disp);
    ui->chart_acc3_d->setRenderHint(QPainter::Antialiasing);

    // 位移时间坐标轴
    QValueAxis *axis_time_disp = new QValueAxis();
    axis_time_disp->setTitleText(tr("时间(s)"));
    axis_time_disp->setTickCount(11);
    axis_time_disp->setMinorTickCount(2);
    axis_time_disp->setRange(0,
                             data_interface->config_.time_count_
                                 / data_interface->config_.frequency_);
    axis_time_disp->setLabelFormat("%f");

    // 位移坐标轴
    QValueAxis *axis_disp = new QValueAxis();
    axis_disp->setTitleText(tr("位移(m)"));
    axis_disp->setTickCount(5);
    axis_disp->setMinorTickCount(2);
    axis_disp->setRange(-0.1, 0.1);
    axis_disp->setLabelFormat("%.2e");

    // 为位移时程绘图对象添加数据
    QLineSeries *mea_disp = chart_data->get_displacement(cur_mea_point);
    mea_disp->setName(tr("测点%1的位移时程").arg(cur_mea_point + 1));
    chart_mea_disp->addSeries(mea_disp);
    // chart_mea_disp->createDefaultAxes();
    chart_mea_disp->addAxis(axis_time_disp, Qt::AlignBottom);
    chart_mea_disp->addAxis(axis_disp, Qt::AlignLeft);
    mea_disp->attachAxis(axis_time_disp);
    mea_disp->attachAxis(axis_disp);
}

// 更新ACC页面加速度、速度和位移的数据
void QRestMainWindow::UpdateAccPage(std::size_t mea_point)
{
    cur_mea_point = mea_point;
    InitAccPage();
}

// 初始化GMP页面
void QRestMainWindow::InitGmpPage()
{
    // 获取测点数量
    std::size_t mea_num = data_interface->config_.mea_number_;
    if (ui->cbox_gmp_mea->count() == 0)
    {
        ui->cbox_gmp_mea->blockSignals(true);
        for (std::size_t i = 1; i <= mea_num; ++i)
        {
            ui->cbox_gmp_mea->addItem(QString::number(i));
        }
        ui->cbox_gmp_mea->blockSignals(false);
    }

    // 创建反应谱绘图对象
    QChart *chart_gmp = new QChart();
    chart_gmp->legend()->setVisible(false);
    ui->chart_gmp_response->setChart(chart_gmp);
    ui->chart_gmp_response->setRenderHint(QPainter::Antialiasing);

    // 周期坐标轴
    QValueAxis *axis_period = new QValueAxis();
    axis_period->setTitleText(tr("周期(s)"));
    axis_period->setTickCount(11);
    axis_period->setMinorTickCount(2);
    axis_period->setRange(0, 5);
    axis_period->setLabelFormat("%f");

    // 反应谱坐标轴
    QValueAxis *axis_response = new QValueAxis();
    axis_response->setTickCount(5);
    axis_response->setMinorTickCount(2);
    axis_response->setRange(0, 0.1);

    // 为反应谱绘图对象添加数据
    QLineSeries *gmp = new QLineSeries();
    switch (gmp_type)
    {
        case 0:
            // 加速度谱
            chart_gmp->setTitle(
                tr("测点%1的加速度反应谱").arg(cur_mea_point + 1));
            axis_response->setTitleText(tr("Sa(m/s^2)"));
            gmp = chart_data->get_sa(cur_mea_point);
            gmp->setName(tr("测点%1的加速度反应谱").arg(cur_mea_point + 1));
            break;
        case 1:
            // 速度谱
            chart_gmp->setTitle(
                tr("测点%1的速度反应谱").arg(cur_mea_point + 1));
            axis_response->setTitleText(tr("Sv(m/s)"));
            gmp = chart_data->get_sv(cur_mea_point);
            gmp->setName(tr("测点%1的速度反应谱").arg(cur_mea_point + 1));
            break;
        case 2:
            // 位移谱
            chart_gmp->setTitle(
                tr("测点%1的位移反应谱").arg(cur_mea_point + 1));
            axis_response->setTitleText(tr("Sd(m)"));
            gmp = chart_data->get_sd(cur_mea_point);
            gmp->setName(tr("测点%1的位移反应谱").arg(cur_mea_point + 1));
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

    chart_gmp->addSeries(gmp);
    // chart_gmp->createDefaultAxes();
    chart_gmp->addAxis(axis_period, Qt::AlignBottom);
    chart_gmp->addAxis(axis_response, Qt::AlignLeft);
    gmp->attachAxis(axis_period);
    gmp->attachAxis(axis_response);

    // 创建频域谱的绘图对象
    QChart *chart_amp = new QChart();
    QChart *chart_pow = new QChart();
    chart_amp->legend()->setVisible(false);
    chart_pow->legend()->setVisible(false);
    ui->chart_gmp_fourier->setChart(chart_amp);
    ui->chart_gmp_fourier->setRenderHint(QPainter::Antialiasing);
    ui->chart_gmp_power->setChart(chart_pow);
    ui->chart_gmp_power->setRenderHint(QPainter::Antialiasing);

    // 幅值谱频率坐标轴
    QValueAxis *axis_amp_freq = new QValueAxis();
    axis_amp_freq->setTitleText(tr("频率(Hz)"));
    axis_amp_freq->setTickCount(11);
    axis_amp_freq->setMinorTickCount(2);
    axis_amp_freq->setRange(0, 5);
    axis_amp_freq->setLabelFormat("%f");

    // 幅值谱坐标轴
    QValueAxis *axis_amp = new QValueAxis();
    axis_amp->setTitleText(tr("幅值"));
    axis_amp->setTickCount(5);
    axis_amp->setMinorTickCount(2);
    axis_amp->setRange(0, 10);
    axis_amp->setLabelFormat("%.2f");

    // 功率谱频率坐标轴
    QValueAxis *axis_pow_freq = new QValueAxis();
    axis_pow_freq->setTitleText(tr("频率(Hz)"));
    axis_pow_freq->setTickCount(11);
    axis_pow_freq->setMinorTickCount(2);
    axis_pow_freq->setRange(0, 5);
    axis_pow_freq->setLabelFormat("%f");

    // 功率谱坐标轴
    QValueAxis *axis_pow = new QValueAxis();
    axis_pow->setTitleText(tr("功率"));
    axis_pow->setTickCount(5);
    axis_pow->setMinorTickCount(2);
    axis_pow->setRange(0, 10);
    axis_pow->setLabelFormat("%.2f");

    // 为频域谱绘图对象添加数据
    QLineSeries *amp = chart_data->get_amplitude(cur_mea_point);
    amp->setName(tr("测点%1的幅值谱").arg(cur_mea_point + 1));
    chart_amp->addSeries(amp);
    // chart_amp->createDefaultAxes();
    chart_amp->addAxis(axis_amp_freq, Qt::AlignBottom);
    chart_amp->addAxis(axis_amp, Qt::AlignLeft);
    amp->attachAxis(axis_amp_freq);
    amp->attachAxis(axis_amp);

    // 为功率谱绘图对象添加数据
    QLineSeries *pow = chart_data->get_power(cur_mea_point);
    pow->setName(tr("测点%1的功率谱").arg(cur_mea_point + 1));
    chart_pow->addSeries(pow);
    // chart_pow->createDefaultAxes();
    chart_pow->addAxis(axis_pow_freq, Qt::AlignBottom);
    chart_pow->addAxis(axis_pow, Qt::AlignLeft);
    pow->attachAxis(axis_pow_freq);
    pow->attachAxis(axis_pow);

    // ui->cbox_gmp_mea->setCurrentIndex(cur_mea_point);
}

// 更新GMP页面反应谱的数据
void QRestMainWindow::UpdateGmpPage(std::size_t mea_point)
{
    cur_mea_point = mea_point;
    InitGmpPage();
}

// 初始化EDP页面
void QRestMainWindow::InitEdpPage()
{
    // 获取楼层数量
    std::size_t floor_num = data_interface->building_.get_floor_height().size();
    if (ui->cbox_edp_floor->count() == 0)
    {
        ui->cbox_edp_floor->blockSignals(true);
        for (std::size_t i = 1; i < floor_num; ++i)
        {
            ui->cbox_edp_floor->addItem(QString::number(i));
        }
        ui->cbox_edp_floor->blockSignals(false);
    }

    // 创建层间位移角绘图对象
    QChart *chart_idr = new QChart();
    chart_idr->setTitle(tr("楼层%1的层间位移角时程").arg(cur_floor + 1));
    chart_idr->legend()->setVisible(false);
    ui->chart_edp_al_idr->setChart(chart_idr);
    ui->chart_edp_al_idr->setRenderHint(QPainter::Antialiasing);

    // 层间位移角时间坐标轴
    QValueAxis *axis_time_idr = new QValueAxis();
    axis_time_idr->setTitleText(tr("时间(s)"));
    axis_time_idr->setTickCount(11);
    axis_time_idr->setMinorTickCount(2);
    axis_time_idr->setRange(0,
                            data_interface->config_.time_count_
                                / data_interface->config_.frequency_);
    axis_time_idr->setLabelFormat("%f");

    // 层间位移角坐标轴
    QValueAxis *axis_idr = new QValueAxis();
    axis_idr->setTitleText(tr("层间位移角"));
    axis_idr->setTickCount(5);
    axis_idr->setMinorTickCount(2);
    axis_idr->setRange(-1.2e-4, 1.2e-4);
    axis_idr->setLabelFormat("%.2e");

    // 为层间位移角绘图对象添加数据
    QLineSeries *idr = chart_data->get_mfi_idr(cur_floor);
    idr->setName(tr("楼层%1的层间位移角时程").arg(cur_floor + 1));
    chart_idr->addSeries(idr);
    // chart_idr->createDefaultAxes();
    chart_idr->addAxis(axis_time_idr, Qt::AlignBottom);
    chart_idr->addAxis(axis_idr, Qt::AlignLeft);
    idr->attachAxis(axis_time_idr);
    idr->attachAxis(axis_idr);

    // 创建位移时程绘图对象
    QChart *chart_disp = new QChart();
    chart_disp->setTitle(tr("楼层%1的位移时程").arg(cur_floor + 1));
    chart_disp->legend()->setVisible(false);
    ui->chart_edp_al_disp->setChart(chart_disp);
    ui->chart_edp_al_disp->setRenderHint(QPainter::Antialiasing);

    // 位移时间坐标轴
    QValueAxis *axis_time_disp = new QValueAxis();
    axis_time_disp->setTitleText(tr("时间(s)"));
    axis_time_disp->setTickCount(11);
    axis_time_disp->setMinorTickCount(2);
    axis_time_disp->setRange(0,
                             data_interface->config_.time_count_
                                 / data_interface->config_.frequency_);
    axis_time_disp->setLabelFormat("%f");

    // 位移坐标轴
    QValueAxis *axis_disp = new QValueAxis();
    axis_disp->setTitleText(tr("位移(m)"));
    axis_disp->setTickCount(5);
    axis_disp->setMinorTickCount(2);
    axis_disp->setRange(-2e-2, 2e-2);
    axis_disp->setLabelFormat("%.2e");

    // 为位移时程绘图对象添加数据
    QLineSeries *disp = chart_data->get_mfi_disp(cur_floor);
    disp->setName(tr("楼层%1的位移时程").arg(cur_floor + 1));
    chart_disp->addSeries(disp);
    // chart_disp->createDefaultAxes();
    chart_disp->addAxis(axis_time_disp, Qt::AlignBottom);
    chart_disp->addAxis(axis_disp, Qt::AlignLeft);
    disp->attachAxis(axis_time_disp);
    disp->attachAxis(axis_disp);

    // 创建层间位移角分布绘图对象
    QChart *chart_all_idr = new QChart();
    chart_all_idr->setTitle(tr("层间位移角分布"));
    chart_all_idr->legend()->setVisible(false);
    ui->chart_edp_al_all_idr->setChart(chart_all_idr);
    ui->chart_edp_al_all_idr->setRenderHint(QPainter::Antialiasing);

    // 层间位移角坐标轴
    QValueAxis *axis_all_idr = new QValueAxis();
    axis_all_idr->setTitleText(tr("层间位移角"));
    axis_all_idr->setTickCount(5);
    axis_all_idr->setMinorTickCount(2);
    axis_all_idr->setRange(0, 1.2e-4);
    axis_all_idr->setLabelFormat("%.2e");

    // 楼层高度坐标轴
    QValueAxis *axis_floor = new QValueAxis();
    axis_floor->setTitleText(tr("楼层高度(m)"));
    axis_floor->setTickCount(11);
    axis_floor->setMinorTickCount(2);
    axis_floor->setRange(data_interface->building_.get_floor_height().front(),
                         data_interface->building_.get_floor_height().back());
    axis_floor->setLabelFormat("%.1f");

    // 为层间位移角分布绘图对象添加数据
    QLineSeries *all_idr = chart_data->get_mfi_all_idr();
    chart_all_idr->addSeries(all_idr);
    // chart_all_idr->createDefaultAxes();
    chart_all_idr->addAxis(axis_all_idr, Qt::AlignBottom);
    chart_all_idr->addAxis(axis_floor, Qt::AlignLeft);
    all_idr->attachAxis(axis_floor);
    all_idr->attachAxis(axis_all_idr);
}

// 更新EDP页面层间位移角和位移的数据
void QRestMainWindow::UpdateEdpPage(std::size_t floor)
{
    cur_floor = floor;
    InitEdpPage();
}

// 初始化Result页面
void QRestMainWindow::InitResultPage()
{
    //
}