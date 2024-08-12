/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\data_visualization\basic_data_visualization.h
** -----
** File Created: Friday, 9th August 2024 13:23:13
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Saturday, 10th August 2024 22:26:08
** Modified By: Dong Feiyue (donfeiyue@outlook.com>)
*/

// Description:
// 数据可视化基类，提供基础数据可视化功能的统一接口。

#ifndef DATA_VISUALIZATION_BASIC_DATA_VISUALIZATION_H
#define DATA_VISUALIZATION_BASIC_DATA_VISUALIZATION_H

// stdc++ headers
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

// third-party library headers
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
    // 缩放比例
    std::vector<double> zoom{0, 0, 1, 1};
    // 图形尺寸
    std::pair<double, double> size = {800, 600};
    // 原点坐标(设为{-1, -1}表示自动设置，值为数据最小点)
    std::pair<double, double> origin = {-1, -1};
    // 样式，默认为蓝色实线，线宽2
    std::string style = "b-2";
    // 是否显示网格
    bool grid = true;
    // 是否显示四周边框
    bool box = true;
};

// 数据可视化基类
class BasicDataVisualization
{
public:
    // 默认构造函数
    BasicDataVisualization() = default;

    // 析构函数
    virtual ~BasicDataVisualization() = default;

    // 绘制函数
    virtual void Draw(mglGraph *gr) = 0;

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

    // 设置缩放比例
    // @param zoom缩放比例参数
    void set_zoom(std::initializer_list<double> zoom)
    {
        if (zoom.size() != 4)
        {
            throw std::invalid_argument("zoom should have 4 elements");
        }
        options_.zoom = {zoom.begin(), zoom.end()};
    }

    // 设置原点坐标
    // @param x x坐标
    // @param y y坐标
    void set_origin(double x, double y) { options_.origin = {x, y}; }

    // 设置图形尺寸
    // @param width 图形宽度
    // @param height 图形高度
    void set_size(double width, double height)
    {
        options_.size = {width, height};
    }

    // 绘制到文件
    void DrawFile(std::string filename = "", const std::string &type = "png");

protected:
    // 可视化选项
    DataVisualizationOptions options_;
};
} // namespace data_visualization
#endif // DATA_VISUALIZATION_BASIC_DATA_VISUALIZATION_H