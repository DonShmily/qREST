/**
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
#include <algorithm>
#include <cmath>
#include <fstream>
#include <memory>

// Qt UI headers
#include "ui_about_dialog.h"
#include "ui_algorithm_dialog.h"

// third-party headers
#include "nlohmann/json.hpp"


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

    // 在新的线程中计算
    // std::thread([this]() {
    // 更新页面
    // UpdateHomePage();
    //}).detach();
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
    QDialog algorithmDialog(this);              // 创建 QDialog 对象
    algorithm_ui_ = new Ui::algorithm_dialog(); // 创建 UI 对象
    algorithm_ui_->setupUi(&algorithmDialog);   // 设置 UI 到 QDialog
    algorithmDialog.exec(); // 以模态对话框的方式显示
    InitAlgorithmDialog();
    // 信号槽连接
    connect(algorithm_ui_->saveButton,
            &QPushButton::clicked,
            this,
            &QRestMainWindow::on_saveButton_clicked);
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
                 static_cast<int>(data_interface_->config_.mea_number_ - 1));
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
        static_cast<int>(data_interface_->building_.get_floor_number() - 2));
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
    page_status_->ResetDirection();
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
    page_status_->ResetDirection();
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
    page_status_->ResetDirection();
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
    page_status_->ResetDirection();
    UpdateEdpPage(cur_floor_);

    qDebug() << "edp方向：" << index;
}

// 初始化算法对话框
void QRestMainWindow::InitAlgorithmDialog()
{
    // 读取配置文件用于显示
    std::ifstream ifs("config/Config.json");
    nlohmann::json config;
    ifs >> config;
    ifs.close();

    // 设置算法对话框的内容
    // 滤波器设置
    algorithm_ui_->gbox_gmp_filter->setChecked(
        config["GMPFilterConfig"]["filter_flag"]);
    algorithm_ui_->sbox_gmp_filter_order->setValue(
        config["GMPFilterConfig"]["filter_order"].get<double>());
    algorithm_ui_->dsbox_gmp_filter_low_freq->setValue(
        config["GMPFilterConfig"]["low_frequency"].get<double>());
    algorithm_ui_->dsbox_gmp_filter_high_freq->setValue(
        config["GMPFilterConfig"]["high_frequency"].get<double>());
    algorithm_ui_->cbox_gmp_filter_type->setCurrentIndex(
        config["GMPFilterConfig"]["filter_type"].get<int>() - 1);
    algorithm_ui_->cbox_gmp_filter_func->setCurrentIndex(
        config["GMPFilterConfig"]["filter_function"].get<int>() - 1);
    algorithm_ui_->cbox_gmp_filter_gen->setCurrentIndex(
        config["GMPFilterConfig"]["filter_generator"].get<int>() - 1);

    // 反应谱设置
    algorithm_ui_->dsbox_gmp_damp->setValue(
        config["ResponseSpectrumConfig"]["damping_ratio"].get<double>());
    algorithm_ui_->dsbox_gmp_period_step->setValue(
        config["ResponseSpectrumConfig"]["period_step"].get<double>());
    algorithm_ui_->dsbox_gmp_max_period->setValue(
        config["ResponseSpectrumConfig"]["max_period"].get<double>());

    // 傅里叶谱设置
    algorithm_ui_->dsbox_gmp_fft_max_period->setValue(
        config["FourierConfig"]["max_frequency"].get<double>());

    // on_saveButton_clicked();
    qDebug() << "Save Button is "
             << (algorithm_ui_->saveButton ? "initialized" : "not initialized");
}

void QRestMainWindow::on_saveButton_clicked()
{
    std::ifstream ifs("config/Config.json");
    nlohmann::json config;
    ifs >> config;
    ifs.close();
    // 调试信息
    qDebug() << "SaveConfig";
    // 保存配置文件
    // 滤波器设置
    config["GMPFilterConfig"]["filter_flag"] =
        algorithm_ui_->gbox_gmp_filter->isChecked();
    config["GMPFilterConfig"]["filter_order"] =
        algorithm_ui_->sbox_gmp_filter_order->value();
    config["GMPFilterConfig"]["low_frequency"] =
        algorithm_ui_->dsbox_gmp_filter_low_freq->value();
    config["GMPFilterConfig"]["high_frequency"] =
        algorithm_ui_->dsbox_gmp_filter_high_freq->value();
    config["GMPFilterConfig"]["filter_type"] =
        algorithm_ui_->cbox_gmp_filter_type->currentIndex() + 1;
    config["GMPFilterConfig"]["filter_function"] =
        algorithm_ui_->cbox_gmp_filter_func->currentIndex() + 1;
    config["GMPFilterConfig"]["filter_generator"] =
        algorithm_ui_->cbox_gmp_filter_gen->currentIndex() + 1;
    qDebug() << config["GMPFilterConfig"]["filter_order"].get<int>();


    // 反应谱设置
    config["ResponseSpectrumConfig"]["damping_ratio"] =
        algorithm_ui_->dsbox_gmp_damp->value();
    config["ResponseSpectrumConfig"]["period_step"] =
        algorithm_ui_->dsbox_gmp_period_step->value();
    config["ResponseSpectrumConfig"]["max_period"] =
        algorithm_ui_->dsbox_gmp_max_period->value();

    // 傅里叶谱设置
    config["FourierConfig"]["max_frequency"] =
        algorithm_ui_->dsbox_gmp_fft_max_period->value();

    // 写入配置文件
    std::ofstream ofs("config/Config.json");
    ofs << config.dump(4);
    ofs.close();
}