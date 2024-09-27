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
** Last Modified: Sunday, 29th September 2024 16:52:23
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description: 主窗口类的实现，计算流程的实现

// assosiated headers
#include "main_window.h"

// stdc++ headers
#include <cstddef>
#include <utility>
#include <vector>

// Qt headers
#include <QtCharts/QtCharts>
#include "qcustomplot.h"

// project headers
#include "chart_data.h"
#include "rectangle_widget.h"

// 初始化主页
void QRestMainWindow::InitHomePage()
{
    // 已完成主页初始化
    page_initialized_->home_page = true;
}

// 更新Home页面
void QRestMainWindow::UpdateHomePage()
{
    //
}

// 初始化Gmp页面
void QRestMainWindow::InitGmpPage()
{
    // 添加三个时程绘图区域
    ui_->widget_gmp_time->plotLayout()->clear();
    QCPAxisRect *time_acc = new QCPAxisRect(ui_->widget_gmp_time);
    time_acc->setupFullAxesBox(true);
    QCPAxisRect *time_vel = new QCPAxisRect(ui_->widget_gmp_time);
    time_vel->setupFullAxesBox(true);
    QCPAxisRect *time_disp = new QCPAxisRect(ui_->widget_gmp_time);
    time_disp->setupFullAxesBox(true);
    ui_->widget_gmp_time->plotLayout()->addElement(0, 0, time_acc);
    ui_->widget_gmp_time->plotLayout()->addElement(1, 0, time_vel);
    ui_->widget_gmp_time->plotLayout()->addElement(2, 0, time_disp);
    ui_->widget_gmp_time->addGraph(time_acc->axis(QCPAxis::atBottom),
                                   time_acc->axis(QCPAxis::atLeft));
    ui_->widget_gmp_time->addGraph(time_vel->axis(QCPAxis::atBottom),
                                   time_vel->axis(QCPAxis::atLeft));
    ui_->widget_gmp_time->addGraph(time_disp->axis(QCPAxis::atBottom),
                                   time_disp->axis(QCPAxis::atLeft));

    // 添加三个反应谱绘图区域
    ui_->widget_gmp_response->plotLayout()->clear();
    QCPAxisRect *response_acc = new QCPAxisRect(ui_->widget_gmp_response);
    response_acc->setupFullAxesBox(true);
    QCPAxisRect *response_vel = new QCPAxisRect(ui_->widget_gmp_response);
    response_vel->setupFullAxesBox(true);
    QCPAxisRect *response_disp = new QCPAxisRect(ui_->widget_gmp_response);
    response_disp->setupFullAxesBox(true);
    ui_->widget_gmp_response->plotLayout()->addElement(0, 0, response_acc);
    ui_->widget_gmp_response->plotLayout()->addElement(0, 1, response_vel);
    ui_->widget_gmp_response->plotLayout()->addElement(0, 2, response_disp);
    ui_->widget_gmp_response->addGraph(response_acc->axis(QCPAxis::atBottom),
                                       response_acc->axis(QCPAxis::atLeft));
    ui_->widget_gmp_response->addGraph(response_vel->axis(QCPAxis::atBottom),
                                       response_vel->axis(QCPAxis::atLeft));
    ui_->widget_gmp_response->addGraph(response_disp->axis(QCPAxis::atBottom),
                                       response_disp->axis(QCPAxis::atLeft));

    // 添加模型绘图区域
    ui_->widget_gmp_model->setNumRectangles(62);

    // 显示信息
    QString msg = "";
    ui_->textBrowser->append(msg);

    // 已完成Gmp页面初始化
    page_initialized_->gmp_page = true;

    // 更新Gmp页面数据
    UpdateGmpPage();
}

// 更新Gmp页面
void QRestMainWindow::UpdateGmpPage()
{
    // 获取数据并转换
    const auto &acc_pnts = chart_data_->get_acceleration(0);
    auto acc_pnts_list = ChartData::PointsVector2DoubleList(acc_pnts);
    const auto &vel_pnts = chart_data_->get_velocity(0);
    auto vel_pnts_list = ChartData::PointsVector2DoubleList(vel_pnts);
    const auto &disp_pnts = chart_data_->get_displacement(0);
    auto disp_pnts_list = ChartData::PointsVector2DoubleList(disp_pnts);

    // 为图表添加数据
    auto data_acc = ui_->widget_gmp_time->axisRects().at(0)->graphs().at(0);
    data_acc->setData(acc_pnts_list->first, acc_pnts_list->second);
    data_acc->rescaleAxes();
    auto data_vel = ui_->widget_gmp_time->axisRects().at(1)->graphs().at(0);
    data_vel->setData(vel_pnts_list->first, vel_pnts_list->second);
    data_vel->rescaleAxes();
    auto data_disp = ui_->widget_gmp_time->axisRects().at(2)->graphs().at(0);
    data_disp->setData(disp_pnts_list->first, disp_pnts_list->second);
    data_disp->rescaleAxes();

    // 获取数据并转换
    const auto &sa_pnts = chart_data_->get_sa(0);
    auto sa_pnts_list = ChartData::PointsVector2DoubleList(sa_pnts);
    const auto &sv_pnts = chart_data_->get_sv(0);
    auto sv_pnts_list = ChartData::PointsVector2DoubleList(sv_pnts);
    const auto &sd_pnts = chart_data_->get_sd(0);
    auto sd_pnts_list = ChartData::PointsVector2DoubleList(sd_pnts);

    // 为图表添加数据
    auto data_sa = ui_->widget_gmp_response->axisRects().at(0)->graphs().at(0);
    data_sa->setData(sa_pnts_list->first, sa_pnts_list->second);
    data_sa->rescaleAxes();
    auto data_sv = ui_->widget_gmp_response->axisRects().at(1)->graphs().at(0);
    data_sv->setData(sv_pnts_list->first, sv_pnts_list->second);
    data_sv->rescaleAxes();
    auto data_sd = ui_->widget_gmp_response->axisRects().at(2)->graphs().at(0);
    data_sd->setData(sd_pnts_list->first, sd_pnts_list->second);
    data_sd->rescaleAxes();
}

// 初始化Mea页面
void QRestMainWindow::InitMeaPage()
{
    InitMeaTabSingle();
    InitMeaTabMultiple();
    // 已完成ACC页面初始化
    page_initialized_->mea_page = true;
}

// 初始化Mea页面下的tab_single
void QRestMainWindow::InitMeaTabSingle()
{
    // 添加三个时程绘图区域
    ui_->widget_mea1_time->plotLayout()->clear();
    QCPAxisRect *time_acc = new QCPAxisRect(ui_->widget_mea1_time);
    time_acc->setupFullAxesBox(true);
    QCPAxisRect *time_vel = new QCPAxisRect(ui_->widget_mea1_time);
    time_vel->setupFullAxesBox(true);
    QCPAxisRect *time_disp = new QCPAxisRect(ui_->widget_mea1_time);
    time_disp->setupFullAxesBox(true);
    ui_->widget_mea1_time->plotLayout()->addElement(0, 0, time_acc);
    ui_->widget_mea1_time->plotLayout()->addElement(1, 0, time_vel);
    ui_->widget_mea1_time->plotLayout()->addElement(2, 0, time_disp);
    ui_->widget_mea1_time->addGraph(time_acc->axis(QCPAxis::atBottom),
                                    time_acc->axis(QCPAxis::atLeft));
    ui_->widget_mea1_time->addGraph(time_vel->axis(QCPAxis::atBottom),
                                    time_vel->axis(QCPAxis::atLeft));
    ui_->widget_mea1_time->addGraph(time_disp->axis(QCPAxis::atBottom),
                                    time_disp->axis(QCPAxis::atLeft));

    // 添加三个反应谱绘图区域
    ui_->widget_mea1_response->plotLayout()->clear();
    QCPAxisRect *response_acc = new QCPAxisRect(ui_->widget_mea1_response);
    response_acc->setupFullAxesBox(true);
    QCPAxisRect *response_vel = new QCPAxisRect(ui_->widget_mea1_response);
    response_vel->setupFullAxesBox(true);
    QCPAxisRect *response_disp = new QCPAxisRect(ui_->widget_mea1_response);
    response_disp->setupFullAxesBox(true);
    ui_->widget_mea1_response->plotLayout()->addElement(0, 0, response_acc);
    ui_->widget_mea1_response->plotLayout()->addElement(1, 0, response_vel);
    ui_->widget_mea1_response->plotLayout()->addElement(2, 0, response_disp);
    ui_->widget_mea1_response->addGraph(response_acc->axis(QCPAxis::atBottom),
                                        response_acc->axis(QCPAxis::atLeft));
    ui_->widget_mea1_response->addGraph(response_vel->axis(QCPAxis::atBottom),
                                        response_vel->axis(QCPAxis::atLeft));
    ui_->widget_mea1_response->addGraph(response_disp->axis(QCPAxis::atBottom),
                                        response_disp->axis(QCPAxis::atLeft));

    // 添加模型绘图区域
    ui_->widget_mea1_model->setNumRectangles(62);

    // 已完成Mea-Single页面下的tab_time初始化
    page_initialized_->mea_tab_single = true;
}

// 更新Mea页面SingleTab
void QRestMainWindow::UpdateMeaTabSingle()
{
    // 获取数据并转换
    const auto &acc_pnts = chart_data_->get_acceleration(cur_mea_point_);
    auto acc_pnts_list = ChartData::PointsVector2DoubleList(acc_pnts);
    const auto &vel_pnts = chart_data_->get_velocity(cur_mea_point_);
    auto vel_pnts_list = ChartData::PointsVector2DoubleList(vel_pnts);
    const auto &disp_pnts = chart_data_->get_displacement(cur_mea_point_);
    auto disp_pnts_list = ChartData::PointsVector2DoubleList(disp_pnts);

    // 为图表添加数据
    auto data_acc = ui_->widget_mea1_time->axisRects().at(0)->graphs().at(0);
    data_acc->setData(acc_pnts_list->first, acc_pnts_list->second);
    data_acc->rescaleAxes();
    auto data_vel = ui_->widget_mea1_time->axisRects().at(1)->graphs().at(0);
    data_vel->setData(vel_pnts_list->first, vel_pnts_list->second);
    data_vel->rescaleAxes();
    auto data_disp = ui_->widget_mea1_time->axisRects().at(2)->graphs().at(0);
    data_disp->setData(disp_pnts_list->first, disp_pnts_list->second);
    data_disp->rescaleAxes();

    // 获取数据并转换
    const auto &sa_pnts = chart_data_->get_sa(cur_mea_point_);
    auto sa_pnts_list = ChartData::PointsVector2DoubleList(sa_pnts);
    const auto &sv_pnts = chart_data_->get_sv(cur_mea_point_);
    auto sv_pnts_list = ChartData::PointsVector2DoubleList(sv_pnts);
    const auto &sd_pnts = chart_data_->get_sd(cur_mea_point_);
    auto sd_pnts_list = ChartData::PointsVector2DoubleList(sd_pnts);

    // 为图表添加数据
    auto data_sa = ui_->widget_mea1_response->axisRects().at(0)->graphs().at(0);
    data_sa->setData(sa_pnts_list->first, sa_pnts_list->second);
    data_sa->rescaleAxes();
    auto data_sv = ui_->widget_mea1_response->axisRects().at(1)->graphs().at(0);
    data_sv->setData(sv_pnts_list->first, sv_pnts_list->second);
    data_sv->rescaleAxes();
    auto data_sd = ui_->widget_mea1_response->axisRects().at(2)->graphs().at(0);
    data_sd->setData(sd_pnts_list->first, sd_pnts_list->second);
    data_sd->rescaleAxes();
}

// 初始化Mea页面下的tab_multiple
void QRestMainWindow::InitMeaTabMultiple()
{
    // 添加所有时程绘图区域
    ui_->widget_mea2_time->plotLayout()->clear();
    for (size_t i = 0; i != data_interface_->config_.mea_number_; ++i)
    {
        // 添加时程绘图区域
        QCPAxisRect *time_acc = new QCPAxisRect(ui_->widget_mea2_time);
        time_acc->setupFullAxesBox(true);
        ui_->widget_mea2_time->plotLayout()->addElement(i, 0, time_acc);
        ui_->widget_mea2_time->addGraph(time_acc->axis(QCPAxis::atBottom),
                                        time_acc->axis(QCPAxis::atLeft));

        // 添加Fourier谱绘图区域
        QCPAxisRect *fourier_acc = new QCPAxisRect(ui_->widget_mea2_fourier);
        fourier_acc->setupFullAxesBox(true);
        ui_->widget_mea2_fourier->plotLayout()->addElement(i, 0, fourier_acc);
        ui_->widget_mea2_fourier->addGraph(fourier_acc->axis(QCPAxis::atBottom),
                                           fourier_acc->axis(QCPAxis::atLeft));
    }

    // 添加模型绘图区域
    ui_->widget_mea2_model->setNumRectangles(62);

    // 已完成Mea-Multiple页面下的tab_time初始化
    page_initialized_->mea_tab_multiple = true;
}

// 更新Mea页面MultipleTab
void QRestMainWindow::UpdateMeaTabMultiple()
{
    for (size_t i = 0; i != data_interface_->config_.mea_number_; ++i)
    {
        // 获取数据并转换
        const auto &acc_pnts = chart_data_->get_acceleration(i);
        auto acc_pnts_list = ChartData::PointsVector2DoubleList(acc_pnts);

        // 为图表添加数据
        auto data_acc =
            ui_->widget_mea2_time->axisRects().at(i)->graphs().at(0);
        data_acc->setData(acc_pnts_list->first, acc_pnts_list->second);
        data_acc->rescaleAxes();

        // 获取数据并转换
        const auto &amp_pnts = chart_data_->get_amplitude(i);
        auto amp_pnts_list = ChartData::PointsVector2DoubleList(amp_pnts);

        // 为图表添加数据
        auto data_amp =
            ui_->widget_mea2_fourier->axisRects().at(i)->graphs().at(0);
        data_amp->setData(amp_pnts_list->first, amp_pnts_list->second);
        data_amp->rescaleAxes();
    }
}


// 初始化SHM页面
void QRestMainWindow::InitShmPage()
{
    // 已完成SHM页面初始化
    page_initialized_->shm_page = true;
}

// 更新SHM页面
void QRestMainWindow::UpdateShmPage()
{
    //
}

// 初始化EDP页面
void QRestMainWindow::InitEdpPage()
{
    // 添加时程绘图区域
    ui_->widget_edp_time->plotLayout()->clear();
    QCPAxisRect *time_acc = new QCPAxisRect(ui_->widget_edp_time);
    time_acc->setupFullAxesBox(true);
    QCPAxisRect *time_disp = new QCPAxisRect(ui_->widget_edp_time);
    time_disp->setupFullAxesBox(true);
    QCPAxisRect *time_idr = new QCPAxisRect(ui_->widget_edp_time);
    time_idr->setupFullAxesBox(true);
    ui_->widget_edp_time->plotLayout()->addElement(0, 0, time_acc);
    ui_->widget_edp_time->plotLayout()->addElement(1, 0, time_disp);
    ui_->widget_edp_time->plotLayout()->addElement(2, 0, time_idr);
    ui_->widget_edp_time->addGraph(time_acc->axis(QCPAxis::atBottom),
                                   time_acc->axis(QCPAxis::atLeft));
    ui_->widget_edp_time->addGraph(time_disp->axis(QCPAxis::atBottom),
                                   time_disp->axis(QCPAxis::atLeft));
    ui_->widget_edp_time->addGraph(time_idr->axis(QCPAxis::atBottom),
                                   time_idr->axis(QCPAxis::atLeft));

    // 添加层间位移角分布绘图区域
    ui_->widget_edp_res->plotLayout()->clear();
    QCPAxisRect *idr_all = new QCPAxisRect(ui_->widget_edp_res);
    idr_all->setupFullAxesBox(true);
    QCPAxisRect *acc_all = new QCPAxisRect(ui_->widget_edp_res);
    acc_all->setupFullAxesBox(true);
    ui_->widget_edp_res->plotLayout()->addElement(0, 0, idr_all);
    ui_->widget_edp_res->plotLayout()->addElement(0, 1, acc_all);
    ui_->widget_edp_res->addGraph(idr_all->axis(QCPAxis::atBottom),
                                  idr_all->axis(QCPAxis::atLeft));
    ui_->widget_edp_res->addGraph(acc_all->axis(QCPAxis::atBottom),
                                  acc_all->axis(QCPAxis::atLeft));

    // 添加模型绘图区域
    ui_->widget_edp_model->setNumRectangles(62);

    // 已完成EDP页面初始化
    page_initialized_->edp_page = true;
}

// 更新EDP页面
void QRestMainWindow::UpdateEdpPage()
{
    // 获取数据并转换
    // 获取数据并转换
    const auto &acc_pnts = chart_data_->get_acceleration(cur_floor_);
    auto acc_pnts_list = ChartData::PointsVector2DoubleList(acc_pnts);
    const auto &disp_pnts = chart_data_->get_mfi_disp(cur_floor_);
    auto disp_pnts_list = ChartData::PointsVector2DoubleList(disp_pnts);
    const auto &idr_pnts = chart_data_->get_mfi_idr(cur_floor_);
    auto idr_pnts_list = ChartData::PointsVector2DoubleList(idr_pnts);

    // 为图表添加数据
    auto data_acc = ui_->widget_edp_time->axisRects().at(0)->graphs().at(0);
    data_acc->setData(acc_pnts_list->first, acc_pnts_list->second);
    data_acc->rescaleAxes();
    auto data_disp = ui_->widget_edp_time->axisRects().at(1)->graphs().at(0);
    data_disp->setData(disp_pnts_list->first, disp_pnts_list->second);
    data_disp->rescaleAxes();
    auto data_idr = ui_->widget_edp_time->axisRects().at(2)->graphs().at(0);
    data_idr->setData(idr_pnts_list->first, idr_pnts_list->second);
    data_idr->rescaleAxes();

    // TODO:以下内容实际只需要更新一次，后面考虑优化
    // 获取数据并转换
    const auto &idr_pnts_all = chart_data_->get_mfi_all_idr();
    auto idr_pnts_all_list = ChartData::PointsVector2DoubleList(idr_pnts_all);
    const auto &acc_pnts_all = chart_data_->get_max_acc();
    auto acc_pnts_all_list = ChartData::PointsVector2DoubleList(acc_pnts_all);

    // 为图表添加数据
    auto data_idr_all = new QCPCurve(
        ui_->widget_edp_res->axisRects().first()->axis(QCPAxis::atBottom),
        ui_->widget_edp_res->axisRects().first()->axis(QCPAxis::atLeft));
    data_idr_all->setData(idr_pnts_all_list->first, idr_pnts_all_list->second);
    data_idr_all->rescaleAxes();
    auto data_acc_all = new QCPCurve(
        ui_->widget_edp_res->axisRects().last()->axis(QCPAxis::atBottom),
        ui_->widget_edp_res->axisRects().last()->axis(QCPAxis::atLeft));
    data_acc_all->setData(acc_pnts_all_list->first, acc_pnts_all_list->second);
    data_acc_all->rescaleAxes();
}