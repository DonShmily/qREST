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

#include "main_window.h"

#include <memory>
#include "QtWidgets/qmessagebox.h"
#include "ui_about_dialog.h"


QRestMainWindow::QRestMainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow())
{
    ui->setupUi(this);
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