/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/data_visualization/plotting_xy.h
// Date: 2024-8
// Author: 董飞跃(Dong Fieyue)
// Description:
// 绘图类，提供二维图形绘制的功能。

#ifndef DATA_VISUALIZATION_PLOTTING_XY_H
#define DATA_VISUALIZATION_PLOTTING_XY_H


// stdc++ headers
#include <vector>

// third-party library headers
// #include "mgl2/glut.h"
#include "mgl2/mgl.h"

// project headers
#include "basic_data_visualization.h"


namespace data_visualization
{

// plot绘图类
class PlotXY : public BasicDataVisualization
{
public:
    // 默认构造函数
    PlotXY() = default;

    // 从x、y数据构造
    // @param x_data x数据
    // @param y_data y数据
    PlotXY(const std::vector<double> &x,
           const std::vector<double> &y,
           const std::string &title = "Title",
           const std::string &x_label = "X",
           const std::string &y_label = "Y")
        : x_data_(x), y_data_(y)
    {
        options_.title = title;
        options_.x_label = x_label;
        options_.y_label = y_label;
    }

    // 析构函数
    ~PlotXY() = default;

    // 绘制函数
    int Draw(mglGraph *gr) override;

private:
    // 图形绘制数据
    std::vector<double> x_data_{};
    std::vector<double> y_data_{};
};
} // namespace data_visualization

#endif // DATA_VISUALIZATION_PLOTTING_XY_H