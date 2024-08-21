#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtCharts/QtCharts>
#include <QtWidgets/QMainWindow>

#include "ui_main_window.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
};
QT_END_NAMESPACE

class QtMainWindow : public QMainWindow
{
    Q_OBJECT

    friend class MainProcess;

public:
    // 构造函数
    QtMainWindow(QWidget *parent = nullptr);
    // 析构函数
    ~QtMainWindow();

    // 传递出chart对象用于获取绘图数据
    QChart *get_chart() { return chart; }

private:
    // 主窗口的UI
    Ui::MainWindow *ui;
    // 图表类
    QChart *chart;

private slots:
    // 菜单栏action的槽函数

    // 文件菜单下的action
    void on_act_open_triggered();
    void on_act_signal_triggered();
    void on_act_exit_triggered();

    // 设置菜单下的action
    void on_act_building_triggered();
    void on_act_algorithm_triggered();
    void on_act_draw_triggered();

    // 帮助菜单下的action
    void on_act_about_triggered();
    void on_act_guide_triggered();
    void on_act_web_triggered();
    void on_act_qt_triggered();

    //
};


#endif // MAIN_WINDOW_H