/**
**            qREST - Quick Response Evaluation for Safety Tagging
**     Institute of Engineering Mechanics, China Earthquake Administration
**
**                 Copyright 2024 - 2024 QLab, Dong Feiyue
**                          All Rights Reserved.
**
** Project: qREST
** File: \src\data_visualization\basic_data_visualization.cpp
** -----
** File Created: Friday, 9th August 2024 13:23:13
** Author: Dong Feiyue (donfeiyue@outlook.com)
** -----
** Last Modified: Saturday, 10th August 2024 22:26:02
** Modified By: Dong Feiyue (donfeiyue@outlook.com>)
*/

// Description:
// 数据可视化基类的实现。

// associated header
#include "basic_data_visualization.h"

// stdc++ headers
#include <initializer_list>
#include <stdexcept>
#include <string>
#include <vector>

// third party headers
#include <mgl2/mgl.h>

namespace data_visualization
{

// 设置xy轴范围
void BasicDataVisualization::set_ranges(std::initializer_list<double> ranges)
{
    {
        if (ranges.size() < 4)
        {
            throw std::invalid_argument("ranges size is at least 4!");
        }
        options_.ranges =
            std::vector<double>(ranges.begin(), ranges.begin() + 4);
        options_.ranges.push_back(0);
        if (ranges.size() >= 5)
        {
            options_.ranges.back() = *(ranges.begin() + 4);
        }
    }
}

// 绘制到文件
void BasicDataVisualization::DrawFile(std::string filename,
                                      const std::string &type)
{
    filename = filename.empty() ? options_.title : filename;
    mglGraph gr;
    Draw(&gr);
    gr.WriteFrame((filename + "." + type).c_str());
}

} // namespace data_visualization