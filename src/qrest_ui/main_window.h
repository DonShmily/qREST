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

// stdc++ headers
#include <cstddef>
#include <memory>

// Qt headers
#include <QtCharts/QtCharts>
#include <QtWidgets/QMainWindow>
#include "qcustomplot.h"

// Qt UI headers
#include "ui_algorithm_dialog.h"
#include "ui_building_dialog.h"
#include "ui_main_window.h"
// #include "ui_mainwindow.h"

// project headers
#include "qrest_calculation/chart_data.h"
#include "qrest_calculation/data_interface.h"

// Qt UIC namespace
QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
};
QT_END_NAMESPACE

// 主窗口中所有页面是否已经完成的结构体
struct PageStatus
{
    bool home_page = false;
    bool gmp_page = false;
    bool mea_page = false;
    bool mea_tab_single = false;
    bool mea_tab_multiple = false;
    bool shm_page = false;
    bool edp_page = false;

    // 更换方向后重置部分页面的状态
    void Reset()
    {
        mea_page = false;
        mea_tab_single = false;
        mea_tab_multiple = false;
        shm_page = false;
        edp_page = false;
    }
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

    // 设置页面UI
    // Ui::algorithm_dialog *algorithm_ui_;

    // 窗口是否已经计算完成的结构体
    std::unique_ptr<PageStatus> page_status_ = std::make_unique<PageStatus>();

    // 数据接口，储存原始数据
    std::shared_ptr<DataInterface> data_interface_ =
        std::make_shared<DataInterface>();
    // 绘图数据对象
    std::unique_ptr<ChartData> chart_data_ = nullptr;

    // 当前选择的方向：0-X方向，1-Y方向，2-Z方向
    int cur_direction_ = 0;
    // 当前页面展示的测点编号
    int cur_mea_point_ = 0;
    // 当前页面展示的楼层编号
    int cur_floor_ = 0;
    // 当前页面展示的反应谱类型
    int gmp_type_ = 0;

    /** 主窗口的私有函数 */

    // 页面初始化
    void Initialize();

    // 初始化Home页面
    void InitHomePage();
    // 更新Home页面
    void UpdateHomePage();
    // 初始化GMP页面
    void InitGmpPage();
    // 更新GMP页面
    void UpdateGmpPage();
    // 初始化Mea页面
    void InitMeaPage();
    // 更新Mea页面
    void UpdateMeaPage(std::size_t new_mea);
    // 初始化SHM页面
    void InitShmPage();
    // 更新SHM页面
    void UpdateShmPage();
    // 初始化EDP页面
    void InitEdpPage();
    // 更新EDP页面
    void UpdateEdpPage(std::size_t new_floor);

    // 初始化和更新各页面下的tab
    // 初始化Mea页面SingleTab
    void InitMeaTabSingle();
    // 更新Mea页面SingleTab
    void UpdateMeaTabSingle(std::size_t new_mea);
    // 初始化Mea页面MultipleTab
    void InitMeaTabMultiple();
    // 更新Mea页面MultipleTab
    void UpdateMeaTabMultiple();

    // 数据/配置更新，刷新所有页面
    void UpdateAllPages();

    // 初始化和更新各对话框
    // 初始化算法对话框
    void InitAlgorithmDialog(Ui::algorithm_dialog *algorithm_ui);
    // 初始化建筑对话框
    void InitBuildingDialog(Ui::building_dialog *building_ui);

private slots:
    /** 菜单栏action的槽函数*/

    // 文件菜单下的action
    void on_act_open_triggered();
    void on_act_signal_triggered();
    void on_act_calc_triggered();

    //// 设置菜单下的action
    void on_act_building_triggered();
    void on_act_algorithm_triggered();

    //// 帮助菜单下的action
    void on_act_about_triggered();
    void on_act_guide_triggered();
    void on_act_web_triggered();
    void on_act_qt_triggered();

    // 上部按钮控制stackedWidget的槽函数
    void on_btn1_main_home_clicked() { ui_->stackedWidget->setCurrentIndex(0); }
    void on_btn2_main_gmp_clicked() { ui_->stackedWidget->setCurrentIndex(1); }
    void on_btn3_main_mea_clicked() { ui_->stackedWidget->setCurrentIndex(2); }
    void on_btn4_main_shm_clicked() { ui_->stackedWidget->setCurrentIndex(3); }
    void on_btn5_main_edp_clicked() { ui_->stackedWidget->setCurrentIndex(4); }

    // 更换stackedWidget后更新相应的页面
    void on_stackedWidget_currentChanged(int index);

    // TabWidget更改后更新相应的页面
    void on_tabWidget_mea_currentChanged(int index);

    // 点击Building模型的槽函数
    void on_widget_mea1_model_fillRectangleClicked(int index);
    void on_widget_edp_model_rectangleClicked(int index);

    // 点击更换方向的comboBox的槽函数
    void on_cbox_mea1_dir_currentIndexChanged(int index);
    void on_cbox_mea2_dir_currentIndexChanged(int index);
    void on_cbox_shm_dir_currentIndexChanged(int index);
    void on_cbox_edp_dir_currentIndexChanged(int index);

    // void on_saveButton_clicked();
};

#endif // MAIN_WINDOW_H