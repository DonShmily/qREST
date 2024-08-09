/**
**           qREST - Quick Response Evaluation for Safety Tagging
**    Institute of Engineering Mechanics, China Earthquake Administration
**
**                           Copyright 2024, QLab
**                           All Rights Reserved.
*/

// File: src/data_visualization/plotting_xy_multi.cpp
// Date: 2024-8
// Author: 董飞跃(Dong Fieyue)
// Description:
// 绘图类的实现，提供一次绘制多条二维图形折线图的功能。

// associated header
#include "plotting_xy_multi.h"

// stdc++ headers
#include <algorithm>
#include <vector>

namespace data_visualization
{
// 绘制函数
int PlotXYM::Draw(mglGraph *gr)
{
    std::vector<double> y_data_min, y_data_max;
    for (const auto &y_data : y_data_)
    {
        // 转换数据为 MathGL 数据格式
        mglData dataX(x_data_.size(), &x_data_[0]);
        mglData dataY(y_data.size(), &y_data[0]);

        // 记录最大最小值
        y_data_min.push_back(*std::min_element(y_data.begin(), y_data.end()));
        y_data_max.push_back(*std::max_element(y_data.begin(), y_data.end()));

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
                std::string style =
                    (idx == std::string::npos
                         ? " o"
                         : std::string(" ") + options_.style[idx]);
                gr->Plot(dataX, dataY, style.c_str());
                break;
        }
    }
    // 设置坐标轴范围，可增加一些边距
    if (options_.ranges.empty())
    {
        options_.ranges.push_back(
            *std::min_element(x_data_.begin(), x_data_.end()));
        options_.ranges.push_back(
            *std::max_element(x_data_.begin(), x_data_.end()));
        options_.ranges.push_back(
            *std::min_element(y_data_min.begin(), y_data_min.end()));
        options_.ranges.push_back(
            *std::max_element(y_data_max.begin(), y_data_max.end()));
    }
    double margin = options_.ranges.size() == 5 ? options_.ranges.back() : 0;
    gr->SetRanges(options_.ranges[0] - margin,
                  options_.ranges[1] + margin,
                  options_.ranges[2] - margin,
                  options_.ranges[3] + margin);

    // 添加标题和轴标签
    gr->Title(options_.title.c_str());
    gr->Label('x', options_.x_label.c_str(), 0);
    gr->Label('y', options_.y_label.c_str(), 0);

    // 绘制坐标轴和网格
    gr->Axis();
    // gr->Grid();

    return 0;
}

} // namespace data_visualization