/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\data_visualization\plotting_xy.cpp
** -----
** File Created: Friday, 9th August 2024 13:23:13
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Saturday, 10th August 2024 22:26:21
** Modified By: Dong Feiyue (donfeiyue@outlook.com>)
*/

// Description:
// 绘图类的实现，提供二维图形绘制的功能。


// associated header
#include "plotting_xy.h"

// stdc++ headers
#include <algorithm>
#include <string>
#include <vector>

#include "basic_data_visualization.h"

#include <mgl2/mgl.h>

namespace data_visualization
{
// 绘制函数
void PlotXY::Draw(mglGraph *gr)
{
    // 设置图形大小
    gr->SetSize(options_.size.first, options_.size.second);

    // 添加标题和轴标签
    gr->Title(options_.title.c_str());
    gr->Label('x', options_.x_label.c_str(), 0);
    gr->Label('y', options_.y_label.c_str(), 0);

    // 设置坐标轴范围，可增加一些边距
    if (options_.ranges.empty())
    {
        options_.ranges.push_back(
            *std::min_element(x_data_.begin(), x_data_.end()));
        options_.ranges.push_back(
            *std::max_element(x_data_.begin(), x_data_.end()));
        options_.ranges.push_back(
            *std::min_element(y_data_.begin(), y_data_.end()));
        options_.ranges.push_back(
            *std::max_element(y_data_.begin(), y_data_.end()));
    }
    double margin_x = options_.ranges.size() > 4 ? options_.ranges[4] : 0,
           margin_y = options_.ranges.size() > 4 ? options_.ranges[5] : 0;
    gr->SetRanges(options_.ranges[0] - margin_x,
                  options_.ranges[1] + margin_x,
                  options_.ranges[2] - margin_x,
                  options_.ranges[3] + margin_x);

    // 设置原点
    if (options_.origin.first == -1 && options_.origin.second == -1)
        options_.origin = {options_.ranges[0], options_.ranges[2]};
    gr->SetOrigin(options_.origin.first, options_.origin.second);

    // 转换数据为 MathGL 数据格式
    mglData dataX(x_data_.size(), &x_data_[0]);
    mglData dataY(y_data_.size(), &y_data_[0]);

    // 绘制二维图形
    switch (options_.type)
    {
        case PlotType::Plot:
            gr->Plot(dataX, dataY, options_.style.c_str());
            break;
        case PlotType::Step:
            gr->Step(dataX, dataY, options_.style.c_str());
            break;
        case PlotType::Stem:
            gr->Stem(dataX, dataY, options_.style.c_str());
            break;
        case PlotType::Bar:
            gr->Bars(dataX, dataY, options_.style.c_str());
            break;
        case PlotType::Scatter:
            auto idx = options_.style.find_last_not_of("o+xsd.v<>^");
            std::string style = (idx == std::string::npos
                                     ? " o"
                                     : std::string(" ") + options_.style[idx]);
            gr->Plot(dataX, dataY, style.c_str());
            break;
    }

    // 设置缩放比例
    gr->Zoom(
        options_.zoom[0], options_.zoom[1], options_.zoom[2], options_.zoom[3]);

    // 绘制坐标轴和网格
    gr->Axis();
    if (options_.box) gr->Box();
    if (options_.grid) gr->Grid();
}

} // namespace data_visualization