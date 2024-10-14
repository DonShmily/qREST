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
** Last Modified: Tuesday, 1st October 2024 17:19:54
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description: 主窗口类的实现，计算流程的实现

// assosiated headers
#include "main_window.h"
#include "QtCore/qnamespace.h"
#include "QtGui/qcolor.h"


// stdc++ headers
#include <cstddef>
#include <utility>

// Qt headers
#include <QtCharts/QtCharts>
#include <vector>
#include "qcustomplot.h"

// project headers
#include "chart_data.h"
#include "rectangle_widget.h"

// 页面初始化
void QRestMainWindow::Initialize()
{
    // 初始化数据接口
    data_interface_->LoadBuilding();
    data_interface_->LoadConfig();

    // 初始化主页
    InitHomePage();
    // 初始化Gmp页面
    InitGmpPage();
    // 初始化Mea页面
    InitMeaPage();
    // 初始化SHM页面
    InitShmPage();
    // 初始化EDP页面
    InitEdpPage();
}

// 初始化主页
void QRestMainWindow::InitHomePage()
{
    // 添加三个时程绘图区域
    ui_->widget_home_time->plotLayout()->clear();
    QCPAxisRect *time_acc_x = new QCPAxisRect(ui_->widget_home_time);
    time_acc_x->setupFullAxesBox(true);
    time_acc_x->axis(QCPAxis::atLeft)->setLabel("X-Acceleration (m/s^2)");
    time_acc_x->axis(QCPAxis::atBottom)->setLabel("Time (s)");
    QCPAxisRect *time_acc_y = new QCPAxisRect(ui_->widget_home_time);
    time_acc_y->setupFullAxesBox(true);
    time_acc_y->axis(QCPAxis::atLeft)->setLabel("Y-Acceleration (m/s^2)");
    time_acc_y->axis(QCPAxis::atBottom)->setLabel("Time (s)");
    QCPAxisRect *time_disp_z = new QCPAxisRect(ui_->widget_home_time);
    time_disp_z->setupFullAxesBox(true);
    time_disp_z->axis(QCPAxis::atLeft)->setLabel("Z-Acceleration (m/s^2)");
    time_disp_z->axis(QCPAxis::atBottom)->setLabel("Time (s)");
    ui_->widget_home_time->plotLayout()->addElement(0, 0, time_acc_x);
    ui_->widget_home_time->plotLayout()->addElement(1, 0, time_acc_y);
    ui_->widget_home_time->plotLayout()->addElement(2, 0, time_disp_z);
    ui_->widget_home_time->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom
                                           | QCP::iSelectPlottables
                                           | QCP::iSelectAxes);

    // 添加反应谱绘图区域
    ui_->widget_home_response->plotLayout()->clear();
    QCPAxisRect *response = new QCPAxisRect(ui_->widget_home_response);
    response->setupFullAxesBox(true);
    response->axis(QCPAxis::atLeft)->setLabel("Sa (m/s^2)");
    ui_->widget_home_response->plotLayout()->addElement(0, 0, response);
    ui_->widget_home_response->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom
                                               | QCP::iSelectPlottables
                                               | QCP::iSelectAxes);

    // 添加EDP绘图区域
    ui_->widget_home_edp->plotLayout()->clear();
    QCPAxisRect *edp_idr = new QCPAxisRect(ui_->widget_home_edp);
    edp_idr->setupFullAxesBox(true);
    edp_idr->axis(QCPAxis::atLeft)->setLabel("Height (m)");
    edp_idr->axis(QCPAxis::atBottom)->setLabel("IDR (rad)");
    QCPAxisRect *edp_acc = new QCPAxisRect(ui_->widget_home_edp);
    edp_acc->setupFullAxesBox(true);
    edp_acc->axis(QCPAxis::atLeft)->setLabel("Height (m)");
    edp_acc->axis(QCPAxis::atBottom)->setLabel("Acceleration (m/s^2)");
    ui_->widget_home_edp->plotLayout()->addElement(0, 0, edp_idr);
    ui_->widget_home_edp->plotLayout()->addElement(0, 1, edp_acc);
    ui_->widget_home_edp->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom
                                          | QCP::iSelectPlottables
                                          | QCP::iSelectAxes);

    // 添加SHM绘图区域
    ui_->widget_home_shm->plotLayout()->clear();
    QCPAxisRect *shm_period = new QCPAxisRect(ui_->widget_home_shm);
    shm_period->setupFullAxesBox(true);
    shm_period->axis(QCPAxis::atLeft)->setLabel("Period (s)");
    shm_period->axis(QCPAxis::atBottom)->setLabel("Time");
    QCPAxisRect *shm_damping = new QCPAxisRect(ui_->widget_home_shm);
    shm_damping->setupFullAxesBox(true);
    shm_damping->axis(QCPAxis::atLeft)->setLabel("Damping Ratio");
    shm_damping->axis(QCPAxis::atBottom)->setLabel("Time");
    ui_->widget_home_shm->plotLayout()->addElement(0, 0, shm_period);
    ui_->widget_home_shm->plotLayout()->addElement(1, 0, shm_damping);
    ui_->widget_home_shm->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom
                                          | QCP::iSelectPlottables
                                          | QCP::iSelectAxes);

    // 添加模型绘图区域
    ui_->widget_home_model->setNumRectangles(
        data_interface_->building_.get_floor_number());
    ui_->widget_home_model->setFillRectangles(
        data_interface_->building_.get_measure_index());
    ui_->widget_home_model->update();

    // 显示信息
    QString msg = "Test";
    ui_->textBrowser->append(msg);
}

// 更新Home页面
void QRestMainWindow::UpdateHomePage()
{
    // 如果Home页面已经计算完成则跳过
    if (page_status_->home_page)
    {
        return;
    }
    std::vector<QColor> colors = {Qt::red, Qt::green, Qt::blue};
    // 获取Gmp数据并转换
    chart_data_->set_direction(0);
    const auto &acc_x_pnts = chart_data_->get_acceleration(0);
    auto acc_x_pnts_list = ChartData::PointsVector2DoubleList(acc_x_pnts);
    const auto &sa_x_pnts = chart_data_->get_sa(0);
    auto sa_x_pnts_list = ChartData::PointsVector2DoubleList(sa_x_pnts);

    chart_data_->set_direction(1);
    const auto &acc_y_pnts = chart_data_->get_acceleration(0);
    auto acc_y_pnts_list = ChartData::PointsVector2DoubleList(acc_y_pnts);
    const auto &sa_y_pnts = chart_data_->get_sa(0);
    auto sa_y_pnts_list = ChartData::PointsVector2DoubleList(sa_y_pnts);

    chart_data_->set_direction(2);
    const auto &acc_z_pnts = chart_data_->get_acceleration(0);
    auto acc_z_pnts_list = ChartData::PointsVector2DoubleList(acc_z_pnts);
    const auto &sa_z_pnts = chart_data_->get_sa(0);
    auto sa_z_pnts_list = ChartData::PointsVector2DoubleList(sa_z_pnts);

    // 重置方向
    chart_data_->set_direction(cur_direction_);

    // 为地震动时程图表添加数据
    ui_->widget_home_time->clearGraphs();
    auto data_acc_x = ui_->widget_home_time->addGraph(
        ui_->widget_home_time->axisRects().at(0)->axis(QCPAxis::atBottom),
        ui_->widget_home_time->axisRects().at(0)->axis(QCPAxis::atLeft));
    data_acc_x->setPen(QPen(colors[0]));
    data_acc_x->setData(acc_x_pnts_list->first, acc_x_pnts_list->second);
    data_acc_x->rescaleAxes();
    auto data_acc_y = ui_->widget_home_time->addGraph(
        ui_->widget_home_time->axisRects().at(1)->axis(QCPAxis::atBottom),
        ui_->widget_home_time->axisRects().at(1)->axis(QCPAxis::atLeft));
    data_acc_y->setPen(QPen(colors[1]));
    data_acc_y->setData(acc_y_pnts_list->first, acc_y_pnts_list->second);
    data_acc_y->rescaleAxes();
    auto data_disp_z = ui_->widget_home_time->addGraph(
        ui_->widget_home_time->axisRects().at(2)->axis(QCPAxis::atBottom),
        ui_->widget_home_time->axisRects().at(2)->axis(QCPAxis::atLeft));
    data_disp_z->setPen(QPen(colors[2]));
    data_disp_z->setData(acc_z_pnts_list->first, acc_z_pnts_list->second);
    data_disp_z->rescaleAxes();

    // 为地震动反应谱图表添加数据
    ui_->widget_home_response->clearGraphs();
    auto data_sa_x = ui_->widget_home_response->addGraph(
        ui_->widget_home_response->axisRects().at(0)->axis(QCPAxis::atBottom),
        ui_->widget_home_response->axisRects().at(0)->axis(QCPAxis::atLeft));
    data_sa_x->setPen(QPen(Qt::red));
    data_sa_x->setData(sa_x_pnts_list->first, sa_x_pnts_list->second);
    data_sa_x->rescaleAxes();
    auto data_sa_y = ui_->widget_home_response->addGraph(
        ui_->widget_home_response->axisRects().at(0)->axis(QCPAxis::atBottom),
        ui_->widget_home_response->axisRects().at(0)->axis(QCPAxis::atLeft));
    data_sa_y->setPen(QPen(Qt::green));
    data_sa_y->setData(sa_y_pnts_list->first, sa_y_pnts_list->second);
    data_sa_y->rescaleAxes(true);
    auto data_sa_z = ui_->widget_home_response->addGraph(
        ui_->widget_home_response->axisRects().at(0)->axis(QCPAxis::atBottom),
        ui_->widget_home_response->axisRects().at(0)->axis(QCPAxis::atLeft));
    data_sa_z->setPen(QPen(Qt::blue));
    data_sa_z->setData(sa_z_pnts_list->first, sa_z_pnts_list->second);
    data_sa_z->rescaleAxes(true);

    // 为EDP图表添加数据
    ui_->widget_home_edp->clearPlottables();
    ui_->widget_home_shm->clearGraphs();
    const auto &idr_limits = chart_data_->get_idr_safty_limit();
    for (const auto &idr_limit : idr_limits)
    {
        auto data_idr_limit = new QCPCurve(
            ui_->widget_home_edp->axisRects().at(0)->axis(QCPAxis::atBottom),
            ui_->widget_home_edp->axisRects().at(0)->axis(QCPAxis::atLeft));
        auto idr_limit_pnts_list =
            ChartData::PointsVector2DoubleList(idr_limit);
        data_idr_limit->setData(idr_limit_pnts_list->first,
                                idr_limit_pnts_list->second);
    }

    // 获取Edp数据并转换
    // ui_->widget_home_edp->axisRects()
    //     .at(0)
    //     ->axis(QCPAxis::atBottom)
    //     ->setRange(0, 0);
    // ui_->widget_home_edp->axisRects()
    //     .at(1)
    //     ->axis(QCPAxis::atBottom)
    //     ->setRange(0, 0);
    for (int i = 0; i != 2; ++i)
    {
        chart_data_->set_direction(i);
        const auto &idr_pnts = chart_data_->get_mfi_all_idr();
        auto idr_pnts_list = ChartData::PointsVector2DoubleList(idr_pnts);
        const auto &acc_pnts = chart_data_->get_max_acc();
        auto acc_pnts_list = ChartData::PointsVector2DoubleList(acc_pnts);
        // const auto &acc_limits = chart_data_->get_acc_safty_limit();

        // 为EDP图表添加数据
        auto data_idr = new QCPCurve(
            ui_->widget_home_edp->axisRects().at(0)->axis(QCPAxis::atBottom),
            ui_->widget_home_edp->axisRects().at(0)->axis(QCPAxis::atLeft));
        data_idr->setData(idr_pnts_list->first, idr_pnts_list->second);
        data_idr->setPen(QPen(colors[i]));
        // data_idr->rescaleAxes(true);

        auto data_acc = new QCPCurve(
            ui_->widget_home_edp->axisRects().at(1)->axis(QCPAxis::atBottom),
            ui_->widget_home_edp->axisRects().at(1)->axis(QCPAxis::atLeft));
        data_acc->setData(acc_pnts_list->first, acc_pnts_list->second);
        data_acc->setPen(QPen(colors[i]));
        // data_acc->rescaleAxes(true);

        // 获取SHM数据并转换
        const auto &period_pnts = chart_data_->get_pi_period();
        auto period_pnts_list = ChartData::PointsVector2DoubleList(period_pnts);
        const auto &damping_pnts = chart_data_->get_pi_damping_ratio();
        auto damping_pnts_list =
            ChartData::PointsVector2DoubleList(damping_pnts);

        // 为SHM图表添加数据
        auto data_period = ui_->widget_home_shm->addGraph(
            ui_->widget_home_shm->axisRects().at(0)->axis(QCPAxis::atBottom),
            ui_->widget_home_shm->axisRects().at(0)->axis(QCPAxis::atLeft));
        data_period->setData(period_pnts_list->first, period_pnts_list->second);
        data_period->setLineStyle(QCPGraph::lsNone);
        data_period->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,
                                                     QPen(colors[i], 2),
                                                     QBrush(Qt::blue),
                                                     10));

        auto data_damping = ui_->widget_home_shm->addGraph(
            ui_->widget_home_shm->axisRects().at(1)->axis(QCPAxis::atBottom),
            ui_->widget_home_shm->axisRects().at(1)->axis(QCPAxis::atLeft));
        data_damping->setData(damping_pnts_list->first,
                              damping_pnts_list->second);
        data_damping->setLineStyle(QCPGraph::lsNone);
        data_damping->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,
                                                      QPen(colors[i], 2),
                                                      QBrush(Qt::blue),
                                                      10));
    }
    ui_->widget_home_edp->rescaleAxes();
    ui_->widget_home_shm->axisRects()
        .at(0)
        ->axis(QCPAxis::atLeft)
        ->setRange(0, 6);
    ui_->widget_home_shm->axisRects()
        .at(1)
        ->axis(QCPAxis::atLeft)
        ->setRange(0, 0.05);

    // 重置方向
    chart_data_->set_direction(cur_direction_);

    // 更新图表内容
    ui_->widget_home_time->replot();
    ui_->widget_home_response->replot();
    ui_->widget_home_edp->replot();
    ui_->widget_home_shm->replot();

    // Home页面计算完成
    page_status_->home_page = true;
}

// 初始化Gmp页面
void QRestMainWindow::InitGmpPage()
{
    // 添加三个方向加速度时程绘图区域
    ui_->widget_gmp_time->plotLayout()->clear();
    QCPAxisRect *time_acc_x = new QCPAxisRect(ui_->widget_gmp_time);
    time_acc_x->setupFullAxesBox(true);
    time_acc_x->axis(QCPAxis::atLeft)->setLabel("X-Acceleration (m/s^2)");
    time_acc_x->axis(QCPAxis::atBottom)->setLabel("Time (s)");
    QCPAxisRect *time_acc_y = new QCPAxisRect(ui_->widget_gmp_time);
    time_acc_y->setupFullAxesBox(true);
    time_acc_y->axis(QCPAxis::atLeft)->setLabel("Y-Acceleration (m/s^2)");
    time_acc_y->axis(QCPAxis::atBottom)->setLabel("Time (s)");
    QCPAxisRect *time_disp_z = new QCPAxisRect(ui_->widget_gmp_time);
    time_disp_z->setupFullAxesBox(true);
    time_disp_z->axis(QCPAxis::atLeft)->setLabel("Z-Acceleration (m/s^2)");
    time_disp_z->axis(QCPAxis::atBottom)->setLabel("Time (s)");
    ui_->widget_gmp_time->plotLayout()->addElement(0, 0, time_acc_x);
    ui_->widget_gmp_time->plotLayout()->addElement(1, 0, time_acc_y);
    ui_->widget_gmp_time->plotLayout()->addElement(2, 0, time_disp_z);
    ui_->widget_gmp_time->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom
                                          | QCP::iSelectPlottables
                                          | QCP::iSelectAxes);

    // 添加三个方向加速度反应谱绘图区域
    ui_->widget_gmp_response->plotLayout()->clear();
    QCPAxisRect *response_acc_x = new QCPAxisRect(ui_->widget_gmp_response);
    response_acc_x->setupFullAxesBox(true);
    response_acc_x->axis(QCPAxis::atLeft)->setLabel("X-Sa (m/s^2)");
    response_acc_x->axis(QCPAxis::atBottom)->setLabel("Period (s)");
    QCPAxisRect *response_acc_y = new QCPAxisRect(ui_->widget_gmp_response);
    response_acc_y->setupFullAxesBox(true);
    response_acc_y->axis(QCPAxis::atLeft)->setLabel("Y-Sa (m/s^2)");
    response_acc_y->axis(QCPAxis::atBottom)->setLabel("Period (s)");
    QCPAxisRect *response_acc_z = new QCPAxisRect(ui_->widget_gmp_response);
    response_acc_z->setupFullAxesBox(true);
    response_acc_z->axis(QCPAxis::atLeft)->setLabel("Z-Sa (m/s^2)");
    response_acc_z->axis(QCPAxis::atBottom)->setLabel("Period (s)");
    ui_->widget_gmp_response->plotLayout()->addElement(0, 0, response_acc_x);
    ui_->widget_gmp_response->plotLayout()->addElement(0, 1, response_acc_y);
    ui_->widget_gmp_response->plotLayout()->addElement(0, 2, response_acc_z);
    ui_->widget_gmp_response->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom
                                              | QCP::iSelectPlottables
                                              | QCP::iSelectAxes);

    // 添加模型绘图区域
    ui_->widget_gmp_model->setNumRectangles(
        data_interface_->building_.get_floor_number());
    ui_->widget_gmp_model->setFillRectangles(
        data_interface_->building_.get_measure_index());
    ui_->widget_gmp_model->update();

    // 显示信息
    QString msg = "";
    ui_->textBrowser->append(msg);
}

// 更新Gmp页面
void QRestMainWindow::UpdateGmpPage()
{
    // 如果Gmp页面已经计算完成则跳过
    if (page_status_->gmp_page)
    {
        return;
    }
    // 获取数据并转换
    chart_data_->set_direction(0);
    const auto &acc_x_pnts = chart_data_->get_acceleration(0);
    auto acc_x_pnts_list = ChartData::PointsVector2DoubleList(acc_x_pnts);
    const auto &sa_x_pnts = chart_data_->get_sa(0);
    auto sa_x_pnts_list = ChartData::PointsVector2DoubleList(sa_x_pnts);

    chart_data_->set_direction(1);
    const auto &acc_y_pnts = chart_data_->get_acceleration(0);
    auto acc_y_pnts_list = ChartData::PointsVector2DoubleList(acc_y_pnts);
    const auto &sa_y_pnts = chart_data_->get_sa(0);
    auto sa_y_pnts_list = ChartData::PointsVector2DoubleList(sa_y_pnts);

    chart_data_->set_direction(2);
    const auto &acc_z_pnts = chart_data_->get_acceleration(0);
    auto acc_z_pnts_list = ChartData::PointsVector2DoubleList(acc_z_pnts);
    const auto &sa_z_pnts = chart_data_->get_sa(0);
    auto sa_z_pnts_list = ChartData::PointsVector2DoubleList(sa_z_pnts);

    // 重置方向
    chart_data_->set_direction(cur_direction_);

    // 为图表添加数据
    ui_->widget_gmp_time->clearGraphs();
    auto data_acc_x = ui_->widget_gmp_time->addGraph(
        ui_->widget_gmp_time->axisRects().at(0)->axis(QCPAxis::atBottom),
        ui_->widget_gmp_time->axisRects().at(0)->axis(QCPAxis::atLeft));
    data_acc_x->setData(acc_x_pnts_list->first, acc_x_pnts_list->second);
    data_acc_x->rescaleAxes();
    auto data_acc_y = ui_->widget_gmp_time->addGraph(
        ui_->widget_gmp_time->axisRects().at(1)->axis(QCPAxis::atBottom),
        ui_->widget_gmp_time->axisRects().at(1)->axis(QCPAxis::atLeft));
    data_acc_y->setData(acc_y_pnts_list->first, acc_y_pnts_list->second);
    data_acc_y->rescaleAxes();
    auto data_acc_z = ui_->widget_gmp_time->addGraph(
        ui_->widget_gmp_time->axisRects().at(2)->axis(QCPAxis::atBottom),
        ui_->widget_gmp_time->axisRects().at(2)->axis(QCPAxis::atLeft));
    data_acc_z->setData(acc_z_pnts_list->first, acc_z_pnts_list->second);
    data_acc_z->rescaleAxes();

    // 为图表添加数据
    ui_->widget_gmp_response->clearGraphs();
    auto data_sa_x = ui_->widget_gmp_response->addGraph(
        ui_->widget_gmp_response->axisRects().at(0)->axis(QCPAxis::atBottom),
        ui_->widget_gmp_response->axisRects().at(0)->axis(QCPAxis::atLeft));
    data_sa_x->setData(sa_x_pnts_list->first, sa_x_pnts_list->second);
    data_sa_x->rescaleAxes();
    auto data_sa_y = ui_->widget_gmp_response->addGraph(
        ui_->widget_gmp_response->axisRects().at(1)->axis(QCPAxis::atBottom),
        ui_->widget_gmp_response->axisRects().at(1)->axis(QCPAxis::atLeft));
    data_sa_y->setData(sa_y_pnts_list->first, sa_y_pnts_list->second);
    data_sa_y->rescaleAxes();
    auto data_sa_z = ui_->widget_gmp_response->addGraph(
        ui_->widget_gmp_response->axisRects().at(2)->axis(QCPAxis::atBottom),
        ui_->widget_gmp_response->axisRects().at(2)->axis(QCPAxis::atLeft));
    data_sa_z->setData(sa_z_pnts_list->first, sa_z_pnts_list->second);
    data_sa_z->rescaleAxes();

    // 更新图表内容
    ui_->widget_gmp_time->replot();
    ui_->widget_gmp_response->replot();

    // Gmp页面计算完成
    page_status_->gmp_page = true;
}

// 初始化Mea页面
void QRestMainWindow::InitMeaPage()
{

    InitMeaTabSingle();
    InitMeaTabMultiple();
}

// 更新Mea页面
void QRestMainWindow::UpdateMeaPage(std::size_t new_mea)
{
    switch (ui_->tabWidget_mea->currentIndex())
    {
        case 0:
            UpdateMeaTabSingle(new_mea);
            break;
        case 1:
            UpdateMeaTabMultiple();
            break;
    }
    page_status_->mea_page = true;
}

// 初始化Mea页面下的tab_single
void QRestMainWindow::InitMeaTabSingle()
{
    // 添加三个时程绘图区域
    ui_->widget_mea1_time->plotLayout()->clear();
    QCPAxisRect *time_acc = new QCPAxisRect(ui_->widget_mea1_time);
    time_acc->setupFullAxesBox(true);
    time_acc->axis(QCPAxis::atLeft)->setLabel("Acceleration (m/s^2)");
    time_acc->axis(QCPAxis::atBottom)->setLabel("Time (s)");
    QCPAxisRect *time_vel = new QCPAxisRect(ui_->widget_mea1_time);
    time_vel->setupFullAxesBox(true);
    time_vel->axis(QCPAxis::atLeft)->setLabel("Velocity (m/s)");
    time_vel->axis(QCPAxis::atBottom)->setLabel("Time (s)");
    QCPAxisRect *time_disp = new QCPAxisRect(ui_->widget_mea1_time);
    time_disp->setupFullAxesBox(true);
    time_disp->axis(QCPAxis::atLeft)->setLabel("Displacement (m)");
    time_disp->axis(QCPAxis::atBottom)->setLabel("Time (s)");
    ui_->widget_mea1_time->plotLayout()->addElement(0, 0, time_acc);
    ui_->widget_mea1_time->plotLayout()->addElement(1, 0, time_vel);
    ui_->widget_mea1_time->plotLayout()->addElement(2, 0, time_disp);
    ui_->widget_mea1_time->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom
                                           | QCP::iSelectPlottables
                                           | QCP::iSelectAxes);

    // 添加三个反应谱绘图区域
    ui_->widget_mea1_response->plotLayout()->clear();
    QCPAxisRect *response_acc = new QCPAxisRect(ui_->widget_mea1_response);
    response_acc->setupFullAxesBox(true);
    response_acc->axis(QCPAxis::atLeft)->setLabel("Sa (m/s^2)");
    response_acc->axis(QCPAxis::atBottom)->setLabel("Period (s)");
    QCPAxisRect *response_vel = new QCPAxisRect(ui_->widget_mea1_response);
    response_vel->setupFullAxesBox(true);
    response_vel->axis(QCPAxis::atLeft)->setLabel("Sv (m/s)");
    response_vel->axis(QCPAxis::atBottom)->setLabel("Period (s)");
    QCPAxisRect *response_disp = new QCPAxisRect(ui_->widget_mea1_response);
    response_disp->setupFullAxesBox(true);
    response_disp->axis(QCPAxis::atLeft)->setLabel("Sd (m)");
    response_disp->axis(QCPAxis::atBottom)->setLabel("Period (s)");
    ui_->widget_mea1_response->plotLayout()->addElement(0, 0, response_acc);
    ui_->widget_mea1_response->plotLayout()->addElement(1, 0, response_vel);
    ui_->widget_mea1_response->plotLayout()->addElement(2, 0, response_disp);
    ui_->widget_mea1_response->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom
                                               | QCP::iSelectPlottables
                                               | QCP::iSelectAxes);

    // 添加模型绘图区域
    ui_->widget_mea1_model->setNumRectangles(
        data_interface_->building_.get_floor_number());
    ui_->widget_mea1_model->setFillRectangles(
        data_interface_->building_.get_measure_index());
    ui_->widget_mea1_model->update();
}

// 更新Mea页面SingleTab
void QRestMainWindow::UpdateMeaTabSingle(std::size_t new_mea)
{
    // 如果测点没有变化且已经完成计算，则跳过
    if (new_mea == cur_mea_point_ && page_status_->mea_tab_single)
    {
        return;
    }
    cur_mea_point_ = new_mea;
    // 获取数据并转换
    const auto &acc_pnts = chart_data_->get_acceleration(cur_mea_point_);
    auto acc_pnts_list = ChartData::PointsVector2DoubleList(acc_pnts);
    const auto &vel_pnts = chart_data_->get_velocity(cur_mea_point_);
    auto vel_pnts_list = ChartData::PointsVector2DoubleList(vel_pnts);
    const auto &disp_pnts = chart_data_->get_displacement(cur_mea_point_);
    auto disp_pnts_list = ChartData::PointsVector2DoubleList(disp_pnts);

    // 为图表添加数据
    ui_->widget_mea1_time->clearGraphs();
    auto data_acc = ui_->widget_mea1_time->addGraph(
        ui_->widget_mea1_time->axisRects().at(0)->axis(QCPAxis::atBottom),
        ui_->widget_mea1_time->axisRects().at(0)->axis(QCPAxis::atLeft));
    data_acc->setData(acc_pnts_list->first, acc_pnts_list->second);
    data_acc->rescaleAxes();
    auto data_vel = ui_->widget_mea1_time->addGraph(
        ui_->widget_mea1_time->axisRects().at(1)->axis(QCPAxis::atBottom),
        ui_->widget_mea1_time->axisRects().at(1)->axis(QCPAxis::atLeft));
    data_vel->setData(vel_pnts_list->first, vel_pnts_list->second);
    data_vel->rescaleAxes();
    auto data_disp = ui_->widget_mea1_time->addGraph(
        ui_->widget_mea1_time->axisRects().at(2)->axis(QCPAxis::atBottom),
        ui_->widget_mea1_time->axisRects().at(2)->axis(QCPAxis::atLeft));
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
    ui_->widget_mea1_response->clearGraphs();
    auto data_sa = ui_->widget_mea1_response->addGraph(
        ui_->widget_mea1_response->axisRects().at(0)->axis(QCPAxis::atBottom),
        ui_->widget_mea1_response->axisRects().at(0)->axis(QCPAxis::atLeft));
    data_sa->setData(sa_pnts_list->first, sa_pnts_list->second);
    data_sa->rescaleAxes();
    auto data_sv = ui_->widget_mea1_response->addGraph(
        ui_->widget_mea1_response->axisRects().at(1)->axis(QCPAxis::atBottom),
        ui_->widget_mea1_response->axisRects().at(1)->axis(QCPAxis::atLeft));
    data_sv->setData(sv_pnts_list->first, sv_pnts_list->second);
    data_sv->rescaleAxes();
    auto data_sd = ui_->widget_mea1_response->addGraph(
        ui_->widget_mea1_response->axisRects().at(2)->axis(QCPAxis::atBottom),
        ui_->widget_mea1_response->axisRects().at(2)->axis(QCPAxis::atLeft));
    data_sd->setData(sd_pnts_list->first, sd_pnts_list->second);
    data_sd->rescaleAxes();

    // 更新图表内容
    ui_->widget_mea1_time->replot();
    ui_->widget_mea1_response->replot();

    // 已完成Mea-Single页面下的tab_single初始化
    page_status_->mea_tab_single = true;
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
        time_acc->axis(QCPAxis::atLeft)->setLabel("Acceleration (m/s^2)");
        time_acc->axis(QCPAxis::atBottom)->setLabel("Time (s)");
        ui_->widget_mea2_time->plotLayout()->addElement(i, 0, time_acc);

        // 添加Fourier谱绘图区域
        QCPAxisRect *fourier_acc = new QCPAxisRect(ui_->widget_mea2_fourier);
        fourier_acc->setupFullAxesBox(true);
        fourier_acc->axis(QCPAxis::atLeft)->setLabel("Amplitude");
        fourier_acc->axis(QCPAxis::atBottom)->setLabel("Frequency (Hz)");
        ui_->widget_mea2_fourier->plotLayout()->addElement(i, 0, fourier_acc);
    }
    ui_->widget_mea2_time->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom
                                           | QCP::iSelectPlottables
                                           | QCP::iSelectAxes);
    ui_->widget_mea2_fourier->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom
                                              | QCP::iSelectPlottables
                                              | QCP::iSelectAxes);
    // 添加模型绘图区域
    ui_->widget_mea2_model->setNumRectangles(
        data_interface_->building_.get_floor_number());
    ui_->widget_mea2_model->setFillRectangles(
        data_interface_->building_.get_measure_index());
    ui_->widget_mea2_model->update();
}

// 更新Mea页面MultipleTab
void QRestMainWindow::UpdateMeaTabMultiple()
{
    // 如果页面已经初始化
    if (page_status_->mea_tab_multiple)
    {
        return;
    }
    std::size_t mea_num = data_interface_->config_.mea_number_;
    ui_->widget_mea2_time->clearGraphs();
    ui_->widget_mea2_fourier->clearGraphs();
    for (size_t i = 0; i != mea_num; ++i)
    {
        // 获取数据并转换
        const auto &acc_pnts = chart_data_->get_acceleration(
            data_interface_->config_.mea_number_ - i - 1);
        auto acc_pnts_list = ChartData::PointsVector2DoubleList(acc_pnts);

        // 为图表添加数据
        auto data_acc = ui_->widget_mea2_time->addGraph(
            ui_->widget_mea2_time->axisRects().at(i)->axis(QCPAxis::atBottom),
            ui_->widget_mea2_time->axisRects().at(i)->axis(QCPAxis::atLeft));
        data_acc->setData(acc_pnts_list->first, acc_pnts_list->second);
        data_acc->rescaleAxes();

        // 获取数据并转换
        const auto &amp_pnts = chart_data_->get_amplitude(
            data_interface_->config_.mea_number_ - i - 1);
        auto amp_pnts_list = ChartData::PointsVector2DoubleList(amp_pnts);

        // 为图表添加数据
        auto data_amp = ui_->widget_mea2_fourier->addGraph(
            ui_->widget_mea2_fourier->axisRects().at(i)->axis(
                QCPAxis::atBottom),
            ui_->widget_mea2_fourier->axisRects().at(i)->axis(QCPAxis::atLeft));
        data_amp->setData(amp_pnts_list->first, amp_pnts_list->second);
        // data_amp->rescaleAxes();
        data_amp->rescaleValueAxis();
        ui_->widget_mea2_fourier->axisRects()
            .at(i)
            ->axis(QCPAxis::atBottom)
            ->setRange(0, 10);
    }

    // 更新图表内容
    ui_->widget_mea2_time->replot();
    ui_->widget_mea2_fourier->replot();

    // 已完成Mea-Multiple页面下的tab_multiple初始化
    page_status_->mea_tab_multiple = true;
}

// 初始化SHM页面
void QRestMainWindow::InitShmPage()
{
    // 添加周期识别结果绘图区域
    ui_->widget_shm_period->plotLayout()->clear();
    QCPAxisRect *period = new QCPAxisRect(ui_->widget_shm_period);
    period->setupFullAxesBox(true);
    period->axis(QCPAxis::atLeft)->setLabel("Period (s)");
    period->axis(QCPAxis::atBottom)->setLabel("Time");
    ui_->widget_shm_period->plotLayout()->addElement(0, 0, period);
    ui_->widget_shm_period->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom
                                            | QCP::iSelectPlottables
                                            | QCP::iSelectAxes);

    // 添加阻尼比识别结果绘图区域
    ui_->widget_shm_damping->plotLayout()->clear();
    QCPAxisRect *damping = new QCPAxisRect(ui_->widget_shm_damping);
    damping->setupFullAxesBox(true);
    damping->axis(QCPAxis::atLeft)->setLabel("Damping Ratio");
    damping->axis(QCPAxis::atBottom)->setLabel("Time");
    ui_->widget_shm_damping->plotLayout()->addElement(0, 0, damping);
    ui_->widget_shm_damping->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom
                                             | QCP::iSelectPlottables
                                             | QCP::iSelectAxes);

    // 添加模态向量识别结果绘图区域
    ui_->widget_shm_mode->plotLayout()->clear();
    QCPAxisRect *mode1 = new QCPAxisRect(ui_->widget_shm_mode);
    mode1->setupFullAxesBox(true);
    mode1->axis(QCPAxis::atLeft)->setLabel("Mode 1");
    QCPAxisRect *mode2 = new QCPAxisRect(ui_->widget_shm_mode);
    mode2->setupFullAxesBox(true);
    mode2->axis(QCPAxis::atLeft)->setLabel("Mode 2");
    ui_->widget_shm_mode->plotLayout()->addElement(0, 0, mode1);
    ui_->widget_shm_mode->plotLayout()->addElement(1, 0, mode2);
    ui_->widget_shm_mode->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom
                                          | QCP::iSelectPlottables
                                          | QCP::iSelectAxes);

    // 添加模型绘图区域
    ui_->widget_shm_model->setNumRectangles(
        data_interface_->building_.get_floor_number());
    ui_->widget_shm_model->setFillRectangles(
        data_interface_->building_.get_measure_index());
    ui_->widget_shm_model->update();
}

// 更新SHM页面
void QRestMainWindow::UpdateShmPage()
{
    // 如果SHM页面已经计算完成则跳过
    if (page_status_->shm_page)
    {
        return;
    }
    // 获取数据并转换
    const auto &period_pnts = chart_data_->get_pi_period();
    auto period_pnts_list = ChartData::PointsVector2DoubleList(period_pnts);
    const auto &damping_pnts = chart_data_->get_pi_damping_ratio();
    auto damping_pnts_list = ChartData::PointsVector2DoubleList(damping_pnts);

    // 为图表添加数据
    ui_->widget_shm_period->clearGraphs();
    ui_->widget_shm_damping->clearGraphs();
    auto data_period = ui_->widget_shm_period->addGraph(
        ui_->widget_shm_period->axisRects().at(0)->axis(QCPAxis::atBottom),
        ui_->widget_shm_period->axisRects().at(0)->axis(QCPAxis::atLeft));
    data_period->setData(period_pnts_list->first, period_pnts_list->second);
    data_period->setLineStyle(QCPGraph::lsNone);
    data_period->setScatterStyle(QCPScatterStyle(
        QCPScatterStyle::ssCircle, QPen(Qt::blue, 2), QBrush(Qt::red), 10));
    ui_->widget_shm_period->axisRects()
        .at(0)
        ->axis(QCPAxis::atLeft)
        ->setRange(0, 6);
    auto data_damping = ui_->widget_shm_damping->addGraph(
        ui_->widget_shm_damping->axisRects().at(0)->axis(QCPAxis::atBottom),
        ui_->widget_shm_damping->axisRects().at(0)->axis(QCPAxis::atLeft));
    data_damping->setData(damping_pnts_list->first, damping_pnts_list->second);
    data_damping->setLineStyle(QCPGraph::lsNone);
    data_damping->setScatterStyle(QCPScatterStyle(
        QCPScatterStyle::ssCircle, QPen(Qt::blue, 2), QBrush(Qt::red), 10));
    ui_->widget_shm_damping->axisRects()
        .at(0)
        ->axis(QCPAxis::atLeft)
        ->setRange(0, 0.05);

    // 更新图表内容
    ui_->widget_shm_period->replot();
    ui_->widget_shm_damping->replot();

    // 已完成SHM页面初始化
    page_status_->shm_page = true;
}

// 初始化EDP页面
void QRestMainWindow::InitEdpPage()
{
    // 添加时程绘图区域
    ui_->widget_edp_time->plotLayout()->clear();
    QCPAxisRect *time_acc = new QCPAxisRect(ui_->widget_edp_time);
    time_acc->setupFullAxesBox(true);
    time_acc->axis(QCPAxis::atLeft)->setLabel("Acceleration (m/s^2)");
    time_acc->axis(QCPAxis::atBottom)->setLabel("Time (s)");
    QCPAxisRect *time_disp = new QCPAxisRect(ui_->widget_edp_time);
    time_disp->setupFullAxesBox(true);
    time_disp->axis(QCPAxis::atLeft)->setLabel("Displacement (m)");
    time_disp->axis(QCPAxis::atBottom)->setLabel("Time (s)");
    QCPAxisRect *time_idr = new QCPAxisRect(ui_->widget_edp_time);
    time_idr->setupFullAxesBox(true);
    time_idr->axis(QCPAxis::atLeft)->setLabel("Interstory Drift Ratio");
    time_idr->axis(QCPAxis::atBottom)->setLabel("Time (s)");
    ui_->widget_edp_time->plotLayout()->addElement(0, 0, time_acc);
    ui_->widget_edp_time->plotLayout()->addElement(1, 0, time_disp);
    ui_->widget_edp_time->plotLayout()->addElement(2, 0, time_idr);
    ui_->widget_edp_time->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom
                                          | QCP::iSelectPlottables
                                          | QCP::iSelectAxes);

    // 添加层间位移角分布绘图区域
    ui_->widget_edp_res->plotLayout()->clear();
    QCPAxisRect *idr_all = new QCPAxisRect(ui_->widget_edp_res);
    idr_all->setupFullAxesBox(true);
    idr_all->axis(QCPAxis::atLeft)->setLabel("Height (m)");
    idr_all->axis(QCPAxis::atBottom)->setLabel("IDR (rad)");
    QCPAxisRect *acc_all = new QCPAxisRect(ui_->widget_edp_res);
    acc_all->setupFullAxesBox(true);
    acc_all->axis(QCPAxis::atLeft)->setLabel("Height (m)");
    acc_all->axis(QCPAxis::atBottom)->setLabel("Acceleration (m/s^2)");
    ui_->widget_edp_res->plotLayout()->addElement(0, 0, idr_all);
    ui_->widget_edp_res->plotLayout()->addElement(0, 1, acc_all);
    ui_->widget_edp_res->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom
                                         | QCP::iSelectPlottables
                                         | QCP::iSelectAxes);

    // 添加模型绘图区域
    ui_->widget_edp_model->setNumRectangles(
        data_interface_->building_.get_floor_number());
    ui_->widget_edp_model->setFillRectangles(
        data_interface_->building_.get_measure_index());
    ui_->widget_edp_model->update();
}

// 更新EDP页面
void QRestMainWindow::UpdateEdpPage(std::size_t new_floor)
{
    // 如果楼层没有变化且已经完成了计算，则跳过
    if (new_floor == cur_floor_ && page_status_->edp_page)
    {
        return;
    }
    cur_floor_ = new_floor;
    // 获取数据并转换
    const auto &acc_pnts = chart_data_->get_mfi_acc(cur_floor_);
    auto acc_pnts_list = ChartData::PointsVector2DoubleList(acc_pnts);
    const auto &disp_pnts = chart_data_->get_mfi_disp(cur_floor_);
    auto disp_pnts_list = ChartData::PointsVector2DoubleList(disp_pnts);
    const auto &idr_pnts = chart_data_->get_mfi_idr(cur_floor_);
    auto idr_pnts_list = ChartData::PointsVector2DoubleList(idr_pnts);

    // 为图表添加数据
    ui_->widget_edp_time->clearPlottables();
    auto data_acc = ui_->widget_edp_time->addGraph(
        ui_->widget_edp_time->axisRects().at(0)->axis(QCPAxis::atBottom),
        ui_->widget_edp_time->axisRects().at(0)->axis(QCPAxis::atLeft));
    data_acc->setData(acc_pnts_list->first, acc_pnts_list->second);
    data_acc->rescaleAxes();
    auto data_disp = ui_->widget_edp_time->addGraph(
        ui_->widget_edp_time->axisRects().at(1)->axis(QCPAxis::atBottom),
        ui_->widget_edp_time->axisRects().at(1)->axis(QCPAxis::atLeft));
    data_disp->setData(disp_pnts_list->first, disp_pnts_list->second);
    data_disp->rescaleAxes();
    auto data_idr = ui_->widget_edp_time->addGraph(
        ui_->widget_edp_time->axisRects().at(2)->axis(QCPAxis::atBottom),
        ui_->widget_edp_time->axisRects().at(2)->axis(QCPAxis::atLeft));
    data_idr->setData(idr_pnts_list->first, idr_pnts_list->second);
    data_idr->rescaleAxes();

    // TODO:以下内容实际只需要更新一次，后面考虑优化
    // 获取数据并转换
    const auto &idr_pnts_all = chart_data_->get_mfi_all_idr();
    auto idr_pnts_all_list = ChartData::PointsVector2DoubleList(idr_pnts_all);
    const auto &acc_pnts_all = chart_data_->get_max_acc();
    auto acc_pnts_all_list = ChartData::PointsVector2DoubleList(acc_pnts_all);

    // 为图表添加数据
    ui_->widget_edp_res->clearGraphs();
    ui_->widget_edp_res->clearPlottables();
    auto data_idr_all = new QCPCurve(
        ui_->widget_edp_res->axisRects().at(0)->axis(QCPAxis::atBottom),
        ui_->widget_edp_res->axisRects().at(0)->axis(QCPAxis::atLeft));
    data_idr_all->setData(idr_pnts_all_list->first, idr_pnts_all_list->second);
    data_idr_all->rescaleAxes();
    auto data_acc_all = new QCPCurve(
        ui_->widget_edp_res->axisRects().at(1)->axis(QCPAxis::atBottom),
        ui_->widget_edp_res->axisRects().at(1)->axis(QCPAxis::atLeft));
    data_acc_all->setData(acc_pnts_all_list->first, acc_pnts_all_list->second);
    data_acc_all->rescaleAxes();

    // 更新图表内容
    ui_->widget_edp_time->replot();
    ui_->widget_edp_res->replot();

    // 已完成EDP页面计算
    page_status_->edp_page = true;
}