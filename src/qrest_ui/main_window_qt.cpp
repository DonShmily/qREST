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
#include <memory>

// Qt UI headers
#include "ui_about_dialog.h"


QRestMainWindow::QRestMainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow())
{
    ui_->setupUi(this);
    ui_->toolBar->setVisible(false);
}

QRestMainWindow::~QRestMainWindow()
{
    if (ui_ != nullptr)
    {
        delete ui_;
    }
}

void QRestMainWindow::on_tabWidget_acc_currentChanged(int index)
{
    switch (index)
    {
        case 0:
            UpdateAccTabTime(cur_mea_point_);
            break;
        case 1:
            UpdateAccTabResponse(cur_mea_point_, gmp_type_);
            break;
        case 2:
            UpdateAccTabFourier(cur_mea_point_);
            break;
        default:
            break;
    }
}

void QRestMainWindow::on_tabWidget_edp_currentChanged(int index)
{
    //
}

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
        data_interface_ =
            std::make_shared<DataInterface>(file_name.toStdString());
    }
    chart_data_ = std::make_unique<ChartData>(data_interface_);
    // 读取文件后根据数据初始化主页
    InitHomePage();

    // 初始化建筑模型
    ui_->widget_building->setNumRectangles(
        data_interface_->building_.get_measuren_height().size());
}

void QRestMainWindow::on_act_about_triggered()
{
    QDialog aboutDialog(this); // 创建 QDialog 对象
    Ui::AboutDialog ui;        // 创建 UI 对象
    ui.setupUi(&aboutDialog);  // 设置 UI 到 QDialog
    aboutDialog.exec();        // 以模态对话框的方式显示
}

void QRestMainWindow::on_act_guide_triggered()
{
    QString pdfPath = "C:/path/to/your/guide.pdf"; // 指定PDF文件的路径
    QUrl pdfUrl = QUrl::fromLocalFile(pdfPath);
    QDesktopServices::openUrl(pdfUrl); // 使用系统默认的PDF阅读器打开文件
}

void QRestMainWindow::on_act_web_triggered()
{
    QUrl url("https://www.qu-zhe.net/qrest.htm"); // 指定要打开的网页URL
    QDesktopServices::openUrl(url);               // 打开指定的网页
}

void QRestMainWindow::on_act_qt_triggered()
{
    QString dlgTitle = "About Qt";
    QMessageBox::aboutQt(this, dlgTitle);
}

void QRestMainWindow::on_cbox_home_mea_currentIndexChanged(int index)
{
    cur_mea_point_ = index;
    UpdateHomePage(index);
}

void QRestMainWindow::on_cbox_acc_mea_currentIndexChanged(int index)
{
    cur_mea_point_ = index;
    UpdateAccTabTime(index);
}

void QRestMainWindow::on_cbox_gmp_mea_currentIndexChanged(int index)
{
    cur_mea_point_ = index;
    UpdateAccTabResponse(index, gmp_type_);
}

void QRestMainWindow::on_cbox_fourier_mea_currentIndexChanged(int index)
{
    cur_mea_point_ = index;
    UpdateAccTabFourier(index);
}

void QRestMainWindow::on_cbox_reponse_currentIndexChanged(int index)
{
    gmp_type_ = index;
    UpdateAccTabResponse(cur_mea_point_, index);
}

void QRestMainWindow::on_cbox_edp_floor_currentIndexChanged(int index)
{
    cur_floor_ = index;
    UpdateEdpTabAlgorithm(cur_floor_);
}

void QRestMainWindow::on_widget_building_rectangleClicked(int index)
{
    cur_floor_ = index;
    switch (ui_->listWidget->currentRow())
    {
        case 0:
            UpdateHomePage(index);
            break;
        case 1:
            switch (ui_->tabWidget_acc->currentIndex())
            {
                case 0:
                    UpdateAccTabTime(index);
                    break;
                case 1:
                    UpdateAccTabResponse(cur_mea_point_, gmp_type_);
                    break;
                case 2:
                    UpdateAccTabFourier(index);
                    break;
                default:
                    break;
            }
            break;
        case 2:
            // UpdateShmPage(index);
            break;
        case 3:
            switch (ui_->tabWidget_edp->currentIndex())
            {
                case 0:
                    UpdateEdpTabRealtime(index);
                    break;
                case 1:
                    UpdateEdpTabAlgorithm(index);
                    break;
                default:
                    break;
            }
            break;
        case 4:
            // UpdateResultPage(index);
            break;
        default:
            break;
    }
}

void QRestMainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    ui_->listWidget->setCurrentRow(currentRow);
    switch (currentRow)
    {
        case 0:
            if (page_initialized_->home_page) break;
            // InitHomePage();
            break;
        case 1:
            if (page_initialized_->acc_page) break;
            InitAccPage();
            break;
        case 2:
            if (page_initialized_->shm_page) break;
            InitShmPage();
            break;
        case 3:
            if (page_initialized_->edp_page) break;
            InitEdpPage();
            break;
        case 4:
            if (page_initialized_->result_page) break;
            InitResultPage();
            break;
        default:
            break;
    }
}