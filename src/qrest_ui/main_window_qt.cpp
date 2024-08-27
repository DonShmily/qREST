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
    : QMainWindow(parent), ui(new Ui::MainWindow())
{
    ui->setupUi(this);
    ui->toolBar->setVisible(false);
}

QRestMainWindow::~QRestMainWindow()
{
    if (ui != nullptr)
    {
        delete ui;
    }
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
        data_interface =
            std::make_unique<DataInterface>(file_name.toStdString());
    }
    chart_data = std::make_unique<ChartData>(
        data_interface->acceleration_data_[0], data_interface->building_);
    // 读取文件后根据数据初始化主页
    InitHomePage();
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
    cur_mea_point = index;
    UpdateHomePage(index);
}

void QRestMainWindow::on_cbox_acc_mea_currentIndexChanged(int index)
{
    cur_mea_point = index;
    UpdateAccPage(index);
}

void QRestMainWindow::on_cbox_gmp_mea_currentIndexChanged(int index)
{
    cur_mea_point = index;
    UpdateGmpPage(index);
}

void QRestMainWindow::on_cbox_reponse_currentIndexChanged(int index)
{
    gmp_type = index;
    UpdateGmpPage(cur_mea_point);
}

void QRestMainWindow::on_cbox_edp_floor_currentIndexChanged(int index)
{
    cur_floor = index;
    UpdateEdpPage(index);
}

void QRestMainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    switch (currentRow)
    {
        case 0:
            // 尚未接收数据，无法初始化该页面
            // InitHomePage();
            break;
        case 1:
            InitAccPage();
            break;
        case 2:
            InitGmpPage();
            break;
        case 3:
            InitEdpPage();
            break;
        case 4:
            InitResultPage();
            break;
        default:
            break;
    }
}