/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\data_visualization\plotting_xy_multi.h
** -----
** File Created: Friday, 9th August 2024 13:23:13
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Saturday, 10th August 2024 22:26:16
** Modified By: Dong Feiyue (donfeiyue@outlook.com>)
*/

// Description:
// 绘图类，提供一次绘制多条二维图形折线图的功能。

#ifndef DATA_VISUALIZATION_PLOTTING_XY_MULTI_H
#define DATA_VISUALIZATION_PLOTTING_XY_MULTI_H

// stdc++ headers
#include <initializer_list>
#include <string>
#include <vector>

// third-party library headers
#include "mgl2/mgl.h"

// project headers
#include "basic_data_visualization.h"


namespace data_visualization
{

// plot绘图类
class PlotXYM : public BasicDataVisualization
{
public:
    // 默认构造函数
    PlotXYM() = default;

    // 从x、y数据构造
    // @param x_data x数据
    // @param y_data y数据
    PlotXYM(const std::vector<double> &x,
            const std::vector<std::vector<double>> &y,
            const std::string &title = "Title",
            const std::string &x_label = "X",
            const std::string &y_label = "Y")
        : x_data_(x), y_data_(y)
    {
        options_.title = title;
        options_.x_label = x_label;
        options_.y_label = y_label;
    }

    // 从初始化列表构造
    // @param x x数据
    // @param y_list y数据列表
    PlotXYM(const std::vector<double> &x,
            std::initializer_list<std::vector<double>> y_list,
            const std::string &title = "Title",
            const std::string &x_label = "X",
            const std::string &y_label = "Y")
        : x_data_(x)
    {
        for (const auto &d : y_list)
        {
            y_data_.push_back(d);
        }
        options_.title = title;
        options_.x_label = x_label;
        options_.y_label = y_label;
    }

    // 析构函数
    ~PlotXYM() = default;

    // 绘制函数
    void Draw(mglGraph *gr) override;

private:
    // 图形绘制数据
    std::vector<double> x_data_{};
    std::vector<std::vector<double>> y_data_{};
};
} // namespace data_visualization

#endif // DATA_VISUALIZATION_PLOTTING_XY_MULTI_H