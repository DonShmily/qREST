/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\qrest_ui\main_window.h
** -----
** File Created: Friday, 16th August 2024 13:34:22
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Monday, 26th August 2024 10:19:50
** Modified By: Dong Feiyue (donfeiyue@outlook.com)
*/

// Description: 定义了主窗口类，继承自QMainWindow，用于显示qREST主窗口的UI界面

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

// Qt headers
#include <memory>
#include <QtCharts/QtCharts>
#include <QtWidgets/QMainWindow>


// UI headers
#include "ui_main_window.h"

// project headers
#include "chart_data.h"
#include "data_interface.h"

// Qt UIC namespace
QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
};
QT_END_NAMESPACE

// qREST主窗口
class QRestMainWindow : public QMainWindow
{
    Q_OBJECT

    friend class MainProcess;

public:
    // 构造函数
    QRestMainWindow(QWidget *parent = nullptr);
    // 析构函数
    ~QRestMainWindow();

private:
    // 主窗口成员变量

    // 主窗口的UI
    Ui::MainWindow *ui;

    // 图表数据
    // 数据接口，储存原始数据
    std::unique_ptr<DataInterface> data_interface;

    // 主窗口的私有函数

    // 初始化Home页面
    void InitHomePage();
    //// 初始化ACC页面
    // void InitAccPage();
    //// 初始化GMP页面
    // void InitGmpPage();
    //// 初始化EDP页面
    // void InitEdpPage();
    //// 初始化Result页面
    // void InitResultPage();

    // 初始化

private slots:
    // 菜单栏action的槽函数

    // 文件菜单下的action
    void on_act_open_triggered();
    // void on_act_signal_triggered();

    //// 设置菜单下的action
    // void on_act_building_triggered();
    // void on_act_algorithm_triggered();
    // void on_act_draw_triggered();

    //// 帮助菜单下的action
    void on_act_about_triggered();
    void on_act_guide_triggered();
    void on_act_web_triggered();
    void on_act_qt_triggered();

    //
};

#endif // MAIN_WINDOW_H