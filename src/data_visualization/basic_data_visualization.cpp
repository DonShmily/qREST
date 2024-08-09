#include "basic_data_visualization.h"

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

// 绘制在GLUT窗口
void BasicDataVisualization::DrawGLUT(std::string title)
{
    title = title.empty() ? options_.title : title;
    mglGLUT gr(this, title.c_str());
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