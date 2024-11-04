﻿/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\qrest_ui\main_window.cpp
** -----
** File Created: Friday, 16th August 2024 13:34:22
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Monday, 26th August 2024 10:21:01
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description: 实现了主窗口类的实现，Qt的UI界面主要逻辑的实现

// assosiated headers
#include "main_window.h"

// stdc++ headers
#include <winsock.h>
#include <algorithm>
#include <cmath>
#include <memory>


// Qt UI headers
#include "ui_about_dialog.h"
#include "ui_algorithm_dialog.h"
#include "ui_building_dialog.h"


// 主窗口类的构造函数
QRestMainWindow::QRestMainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow())
{
    ui_->setupUi(this);

    // 初始化页面初始化状态
    Initialize();
}

// 主窗口类的析构函数
QRestMainWindow::~QRestMainWindow()
{
    if (ui_ != nullptr)
    {
        delete ui_;
    }
}

// open操作
void QRestMainWindow::on_act_open_triggered()
{
    // 打开文件对话框
    QString file_name = QFileDialog::getOpenFileName(
        this, tr("Open File"), "", tr("All Files(*.*);;Text Files(*.txt)"));
    if (file_name.isEmpty())
    {
        QMessageBox::warning(this, tr("Warning"), tr("No file selected!"));
    }
    else
    {
        // 添加加速度数据
        data_interface_->ReadFile(file_name.toStdString());
        chart_data_ = std::make_unique<ChartData>(data_interface_);
    }
}

void QRestMainWindow::on_act_signal_triggered()
{
    // 读取串口数据，暂时不实现
    // 目前读取确定的文件，按照时间窗口展示数据
    // 打开文件对话框
    QString file_name = QFileDialog::getOpenFileName(
        this, tr("Open File"), "", tr("All Files(*.*);;Text Files(*.txt)"));
    if (file_name.isEmpty())
    {
        QMessageBox::warning(this, tr("Warning"), tr("No file selected!"));
        return;
    }
    // 添加加速度数据

    while (data_interface_->time_idx_
           < data_interface_->data_config_.time_count_
                 - data_interface_->data_config_.time_window_)
    {
        data_interface_->ReadFileTimeWindow(file_name.toStdString());
        chart_data_ = std::make_unique<ChartData>(data_interface_);
        UpdateAllPages();

        // 等待一段时间
        QTime dieTime = QTime::currentTime().addMSecs(1000);
        while (QTime::currentTime() < dieTime)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
        // Sleep(1000);
    }

    // data_interface_->ReadFileTimeWindow(file_name.toStdString());
    // chart_data_ = std::make_unique<ChartData>(data_interface_);
}

// 计算操作
void QRestMainWindow::on_act_calc_triggered()
{
    if (chart_data_ == nullptr)
    {
        return;
    }
    UpdateHomePage();
}

// algorithm操作
void QRestMainWindow::on_act_algorithm_triggered()
{
    // 打开算法设置对话框
    QDialog algorithmDialog(this);                   // 创建 QDialog 对象
    auto algorithm_ui_ = new Ui::algorithm_dialog(); // 创建 UI 对象
    algorithm_ui_->setupUi(&algorithmDialog);        // 设置 UI 到 QDialog
    InitAlgorithmDialog(algorithm_ui_);
    algorithmDialog.exec(); // 以模态对话框的方式显示
}

// building操作
void QRestMainWindow::on_act_building_triggered()
{
    // 打开建筑信息对话框
    QDialog buildingDialog(this);         // 创建 QDialog 对象
    Ui::building_dialog building_ui;      // 创建 UI 对象
    building_ui.setupUi(&buildingDialog); // 设置 UI 到 QDialog
    InitBuildingDialog(&building_ui);     // 初始化建筑信息对话框
    buildingDialog.exec();                // 以模态对话框的方式显示
}

// about操作
void QRestMainWindow::on_act_about_triggered()
{
    QDialog aboutDialog(this); // 创建 QDialog 对象
    Ui::AboutDialog ui;        // 创建 UI 对象
    ui.setupUi(&aboutDialog);  // 设置 UI 到 QDialog
    aboutDialog.exec();        // 以模态对话框的方式显示
}

// guide操作
void QRestMainWindow::on_act_guide_triggered()
{
    QString pdfPath = "C:/path/to/your/guide.pdf"; // 指定PDF文件的路径
    QUrl pdfUrl = QUrl::fromLocalFile(pdfPath);
    QDesktopServices::openUrl(pdfUrl); // 使用系统默认的PDF阅读器打开文件
}

// web操作
void QRestMainWindow::on_act_web_triggered()
{
    QUrl url("https://www.qu-zhe.net/qrest.htm"); // 指定要打开的网页URL
    QDesktopServices::openUrl(url);               // 打开指定的网页
}

// qt操作
void QRestMainWindow::on_act_qt_triggered()
{
    QString dlgTitle = "About Qt";
    QMessageBox::aboutQt(this, dlgTitle);
}

// stackWidget页面切换
void QRestMainWindow::on_stackedWidget_currentChanged(int index)
{
    // 如果没有绘图数据，不执行计算
    if (chart_data_ == nullptr)
    {
        return;
    }
    // 根据索引更新相应的页面
    switch (index)
    {
        case 0:
            UpdateHomePage();
            break;
        case 1:
            UpdateGmpPage();
            break;
        case 2:
            UpdateMeaPage(cur_mea_point_);
            break;
        case 3:
            UpdateShmPage();
            break;
        case 4:
            UpdateEdpPage(cur_floor_);
            break;
    }
}

// Mea的tab页面切换
void QRestMainWindow::on_tabWidget_mea_currentChanged(int index)
{
    // 如果没有绘图数据，不执行计算
    if (chart_data_ == nullptr)
    {
        return;
    }
    UpdateMeaPage(cur_mea_point_);
}

// Mea页面1的模型点击事件
void QRestMainWindow::on_widget_mea1_model_fillRectangleClicked(int index)
{
    if (chart_data_ == nullptr)
    {
        return;
    }
    // 点击非填充区域不执行操作
    if (index == -1)
    {
        return;
    }
    index =
        std::min(std::max(index, 0),
                 static_cast<int>(
                     data_interface_->config_->data_measurement_number - 1));
    UpdateMeaTabSingle(index);

    qDebug() << "mea1测点：" << index;
}

// Edp页面的模型点击事件
void QRestMainWindow::on_widget_edp_model_rectangleClicked(int index)
{
    if (chart_data_ == nullptr)
    {
        return;
    }
    index = std::min(
        std::max(index, 0),
        static_cast<int>(data_interface_->building_->get_floor_number() - 2));
    UpdateEdpPage(index);

    qDebug() << "edp楼层：" << index;
}

// Mea页面1的方向切换
void QRestMainWindow::on_cbox_mea1_dir_currentIndexChanged(int index)
{
    if (chart_data_ == nullptr)
    {
        return;
    }
    chart_data_->set_direction(index);
    page_status_->Reset();
    UpdateMeaTabSingle(cur_mea_point_);

    qDebug() << "mea1方向：" << index;
}

// Mea页面2的方向切换
void QRestMainWindow::on_cbox_mea2_dir_currentIndexChanged(int index)
{
    if (chart_data_ == nullptr)
    {
        return;
    }
    chart_data_->set_direction(index);
    page_status_->Reset();
    UpdateMeaTabMultiple();

    qDebug() << "mea2方向：" << index;
}

// Shm页面的方向切换
void QRestMainWindow::on_cbox_shm_dir_currentIndexChanged(int index)
{
    if (chart_data_ == nullptr)
    {
        return;
    }
    chart_data_->set_direction(index);
    page_status_->Reset();
    UpdateShmPage();

    qDebug() << "shm方向：" << index;
}

// Edp页面的方向切换
void QRestMainWindow::on_cbox_edp_dir_currentIndexChanged(int index)
{
    if (chart_data_ == nullptr)
    {
        return;
    }
    chart_data_->set_direction(index);
    page_status_->Reset();
    UpdateEdpPage(cur_floor_);

    qDebug() << "edp方向：" << index;
}

// 初始化算法设置页面
void QRestMainWindow::InitAlgorithmDialog(Ui::algorithm_dialog *algorithm_ui)
{
    // GMP滤波参数
    algorithm_ui->cbox_gmp_filter_type->setCurrentIndex(
        data_interface_->config_->gmp_filter_type);
    algorithm_ui->cbox_gmp_filter_func->setCurrentIndex(
        data_interface_->config_->gmp_filter_function);
    algorithm_ui->cbox_gmp_filter_gen->setCurrentIndex(
        data_interface_->config_->gmp_filter_generator);
    algorithm_ui->dsbox_gmp_filter_low_freq->setValue(
        data_interface_->config_->gmp_low_frequency);
    algorithm_ui->dsbox_gmp_filter_high_freq->setValue(
        data_interface_->config_->gmp_high_frequency);
    algorithm_ui->sbox_gmp_filter_order->setValue(
        data_interface_->config_->gmp_filter_order);

    // GMP反应谱参数
    algorithm_ui->dsbox_gmp_damp->setValue(
        data_interface_->config_->gmp_damping_ratio);
    algorithm_ui->dsbox_gmp_max_period->setValue(
        data_interface_->config_->gmp_max_period);
    algorithm_ui->dsbox_gmp_period_step->setValue(
        data_interface_->config_->gmp_period_step);
    algorithm_ui->dsbox_gmp_fft_max_period->setValue(
        data_interface_->config_->gmp_max_frequency);

    // EDP滤波参数
    algorithm_ui->cbox_edp_method->setCurrentIndex(
        data_interface_->config_->edp_method);
    algorithm_ui->cbox_edp_filter_type->setCurrentIndex(
        data_interface_->config_->edp_filter_type);
    algorithm_ui->cbox_edp_filter_func->setCurrentIndex(
        data_interface_->config_->edp_filter_function);
    algorithm_ui->cbox_edp_filter_gen->setCurrentIndex(
        data_interface_->config_->edp_filter_generator);
    algorithm_ui->dsbox_edp_filter_high_freq->setValue(
        data_interface_->config_->edp_high_frequency);
    algorithm_ui->sbox_edp_filter_order->setValue(
        data_interface_->config_->edp_filter_order);
    algorithm_ui->cbox_edp_filter_type_2->setCurrentIndex(
        data_interface_->config_->edp_filter_type);
    algorithm_ui->cbox_edp_filter_func_2->setCurrentIndex(
        data_interface_->config_->edp_filter_function);
    algorithm_ui->cbox_edp_filter_gen_2->setCurrentIndex(
        data_interface_->config_->edp_filter_generator);
    algorithm_ui->dsbox_edp_filter_high_freq_2->setValue(
        data_interface_->config_->edp_high_frequency);
    algorithm_ui->sbox_edp_filter_order_2->setValue(
        data_interface_->config_->edp_filter_order);
    algorithm_ui->cbox_edp_interp_method->setCurrentIndex(
        data_interface_->config_->edp_interp_method);


    // 点击保存按钮后更新配置并保存至文件
    connect(algorithm_ui->saveButton, &QPushButton::clicked, [=]() {
        // GMP滤波参数
        data_interface_->config_->gmp_filter_type =
            algorithm_ui->cbox_gmp_filter_type->currentIndex();
        data_interface_->config_->gmp_filter_function =
            algorithm_ui->cbox_gmp_filter_func->currentIndex();
        data_interface_->config_->gmp_filter_generator =
            algorithm_ui->cbox_gmp_filter_gen->currentIndex();
        data_interface_->config_->gmp_low_frequency =
            algorithm_ui->dsbox_gmp_filter_low_freq->value();
        data_interface_->config_->gmp_high_frequency =
            algorithm_ui->dsbox_gmp_filter_high_freq->value();
        data_interface_->config_->gmp_filter_order =
            algorithm_ui->sbox_gmp_filter_order->value();

        // GMP反应谱参数
        data_interface_->config_->gmp_damping_ratio =
            algorithm_ui->dsbox_gmp_damp->value();
        data_interface_->config_->gmp_max_period =
            algorithm_ui->dsbox_gmp_max_period->value();
        data_interface_->config_->gmp_period_step =
            algorithm_ui->dsbox_gmp_period_step->value();
        data_interface_->config_->gmp_max_frequency =
            algorithm_ui->dsbox_gmp_fft_max_period->value();

        // EDP滤波参数
        data_interface_->config_->edp_method =
            algorithm_ui->cbox_edp_method->currentIndex();
        data_interface_->config_->edp_filter_type =
            algorithm_ui->cbox_edp_filter_type->currentIndex();
        data_interface_->config_->edp_filter_function =
            algorithm_ui->cbox_edp_filter_func->currentIndex();
        data_interface_->config_->edp_filter_generator =
            algorithm_ui->cbox_edp_filter_gen->currentIndex();
        data_interface_->config_->edp_high_frequency =
            algorithm_ui->dsbox_edp_filter_high_freq->value();
        data_interface_->config_->edp_filter_order =
            algorithm_ui->sbox_edp_filter_order->value();
        data_interface_->config_->edp_interp_method =
            algorithm_ui->cbox_edp_interp_method->currentIndex();

        // 保存配置
        data_interface_->config_->save_config("config/Config.json");

        // 重置计算状态
        UpdateAllPages();
    });
}

// 初始化建筑信息对话框
void QRestMainWindow::InitBuildingDialog(Ui::building_dialog *building_ui)
{
    // 初始化楼层数量
    building_ui->sbox_floor_count->setValue(
        data_interface_->building_->get_floor_number());
    for (const auto &floor : data_interface_->building_->get_floor_height())
    {
        building_ui->text_floor_height->append(QString::number(floor));
    }
    for (const auto &mea : data_interface_->building_->get_measuren_height())
    {
        building_ui->text_mea_height->append(QString::number(mea));
    }
    for (const auto mea : data_interface_->building_->get_measure_index())
    {
        building_ui->text_mea_idx->append(QString::number(mea));
    }
    building_ui->widget_model->setNumRectangles(
        data_interface_->building_->get_floor_number());
    building_ui->widget_model->setFillRectangles(
        data_interface_->building_->get_measure_index());
    building_ui->widget_model->update();

    // 点击保存按钮后更新配置并保存至文件
    connect(building_ui->saveButton, &QPushButton::clicked, [=]() {
        // 重置计算状态
        UpdateAllPages();
    });
}