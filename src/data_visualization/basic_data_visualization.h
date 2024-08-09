/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/data_visualization/basic_data_visualization.h
// Date: 2024-8
// Author: 董飞跃(Dong Fieyue)
// Description:
// 数据可视化基类，提供基础数据可视化功能的统一接口。

#ifndef DATA_VISUALIZATION_BASIC_DATA_VISUALIZATION_H
#define DATA_VISUALIZATION_BASIC_DATA_VISUALIZATION_H


// stdc++ headers
#include <initializer_list>
#include <string>
#include <vector>

// third-party library headers
#include "mgl2/glut.h"
#include "mgl2/mgl.h"


namespace data_visualization
{

// 绘图类型
enum class PlotType
{
    // 折线图
    Plot,
    // 阶梯图
    Step,
    // 柱状图
    Stem,
    // 条形图
    Bar,
    // 散点图
    Scatter
};

// 数据可视化选项
struct DataVisualizationOptions
{
    // 绘图类型
    PlotType type = PlotType::Plot;
    // 标题
    std::string title;
    // x轴标签
    std::string x_label;
    // y轴标签
    std::string y_label;
    // xy轴范围
    std::vector<double> ranges;
    // 样式，默认为蓝色实线，线宽2
    std::string style = "b-2";
};

// 数据可视化基类
class BasicDataVisualization : public mglDraw
{
public:
    // 默认构造函数
    BasicDataVisualization() = default;

    // 析构函数
    virtual ~BasicDataVisualization() = default;

    // 绘制函数
    virtual int Draw(mglGraph *gr) = 0;

    // 设置标题
    // @param title 标题
    void set_title(const std::string &title) { options_.title = title; }
    // 设置x轴标签
    // @param label x轴标签
    void set_x_label(const std::string &label) { options_.x_label = label; }
    // 设置y轴标签
    // @param label y轴标签
    void set_y_label(const std::string &label) { options_.y_label = label; }
    // 设置样式
    // @param style 样式
    void set_style(const std::string &style) { options_.style = style; }
    // 设置xy轴范围
    // @param ranges xy轴范围 {xmin, xmax, ymin, ymax, margin = 0}
    void set_ranges(std::initializer_list<double> ranges);

    // 绘制在GLUT窗口
    void DrawGLUT(std::string title = "");

    // 绘制到文件
    void DrawFile(std::string filename = "", const std::string &type = "png");

protected:
    // 可视化选项
    DataVisualizationOptions options_;
};
} // namespace data_visualization
#endif // DATA_VISUALIZATION_BASIC_DATA_VISUALIZATION_H