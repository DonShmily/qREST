﻿/**
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

// stdc++ headers
#include <cstddef>
#include <memory>

// Qt headers
#include <QtCharts/QtCharts>
#include <QtWidgets/QMainWindow>

// Qt UI headers
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

// 主窗口中所有页面是否已经初始化的结构体
struct PageInitStatus
{
    bool home_page = false;
    bool acc_page = false;
    bool acc_tab_time = false;
    bool acc_tab_response = false;
    bool acc_tab_fourier = false;
    bool shm_page = false;
    bool edp_page = false;
    bool edp_tab_realtime = false;
    bool edp_tab_algorithm = false;
    bool result_page = false;
};

// qREST主窗口
class QRestMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // 构造函数
    QRestMainWindow(QWidget *parent = nullptr);
    // 析构函数
    ~QRestMainWindow();

private:
    // 主窗口成员变量

    // 主窗口的UI
    Ui::MainWindow *ui_;

    // 窗口是否已经被初始化
    std::unique_ptr<PageInitStatus> page_initialized_ =
        std::make_unique<PageInitStatus>();

    // 数据接口，储存原始数据
    std::shared_ptr<DataInterface> data_interface_{};
    // 绘图数据对象
    std::unique_ptr<ChartData> chart_data_{};

    // 当前选择的方向：0-X方向，1-Y方向，2-Z方向
    int cur_direction_ = 0;
    // 当前页面展示的测点编号
    int cur_mea_point_ = 0;
    // 当前页面展示的楼层编号
    int cur_floor_ = 0;
    // 当前页面展示的反应谱类型
    int gmp_type_ = 0;

    /** 主窗口的私有函数 */

    // 初始化Home页面
    void InitHomePage();
    // 更新Home页面
    void UpdateHomePage(std::size_t mea_point);
    // 初始化ACC页面
    void InitAccPage();
    // 初始化SHM页面
    void InitShmPage();
    // 初始化EDP页面
    void InitEdpPage();
    // 初始化Result页面
    void InitResultPage();

    // 初始化和更新各页面下的tab
    // 初始化ACC页面下的tab_time
    void InitAccTabTime();
    // 更新ACC页面下的tab_time
    void UpdateAccTabTime(std::size_t mea_point);
    // 初始化ACC页面下的tab_response
    void InitAccTabResponse();
    // 更新ACC页面下的tab_response
    void UpdateAccTabResponse(std::size_t mea_point, std::size_t gmp_type);
    // 初始化ACC页面下的tab_fourier
    void InitAccTabFourier();
    // 更新ACC页面下的tab_fourier
    void UpdateAccTabFourier(std::size_t mea_point);
    // 初始化EDP页面下的tab_realtime
    void InitEdpTabRealtime();
    // 更新EDP页面下的tab_realtime
    void UpdateEdpTabRealtime(std::size_t mea_point);
    // 初始化EDP页面下的tab_algorithm
    void InitEdpTabAlgorithm();
    // 更新EDP页面下的tab_algorithm
    void UpdateEdpTabAlgorithm(std::size_t mea_point);

private slots:
    /** 菜单栏action的槽函数*/

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

    // ListWidget更改后更新相应的页面
    void on_listWidget_currentRowChanged(int currentRow);

    // TabWidget更改后更新相应的页面
    void on_tabWidget_acc_currentChanged(int index);
    void on_tabWidget_edp_currentChanged(int index);

    // 更换当前测点/楼层的ComboBox槽函数
    void on_cbox_home_mea_currentIndexChanged(int index);
    void on_cbox_acc_mea_currentIndexChanged(int index);
    void on_cbox_gmp_mea_currentIndexChanged(int index);
    void on_cbox_fourier_mea_currentIndexChanged(int index);
    void on_cbox_reponse_currentIndexChanged(int index);
    void on_cbox_edp_floor_currentIndexChanged(int index);

    // 点击Building模型的槽函数
    void on_widget_building_rectangleClicked(int index);
};

#endif // MAIN_WINDOW_H