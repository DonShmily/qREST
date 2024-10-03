#include "rectangle_widget.h"

void RectangleWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // 默认设置线宽为 2 像素
    QPen pen;
    pen.setWidth(2);         // 设置线宽
    pen.setColor(Qt::black); // 设置线条颜色
    painter.setPen(pen);

    // 根据窗口的宽高计算图像区域的大小
    figureWidth = width() / 2;
    figureHeight = height() * 9 / 10;

    // 每个矩形的高度和宽度
    rectHeight = figureHeight / (numRectangles == 0 ? 1 : numRectangles);
    rectWidth = figureWidth;

    // 绘制每个矩形
    for (int i = 0; i < numRectangles; ++i)
    {
        QRect rect(rectWidth / 2,
                   i * rectHeight + figureHeight / 20,
                   rectWidth,
                   rectHeight);
        painter.drawRect(rect);

        if (std::find(fillIndices.begin(),
                      fillIndices.end(),
                      numRectangles - (i + 1))
            != fillIndices.end()) // 如果是需要填充颜色的矩形
        {
            painter.fillRect(rect, fillColor);
        }

        if (std::find(borderIndices.begin(),
                      borderIndices.end(),
                      numRectangles - (i + 1))
            != borderIndices.end()) // 如果是需要修改下边界颜色的矩形
        {
            QPen borderPen(borderColor);
            borderPen.setWidth(10);
            painter.setPen(borderPen);
            painter.drawLine(rect.bottomLeft(), rect.bottomRight());
            painter.setPen(pen); // 恢复原来的画笔
        }

        if (numRectangles - (i + 1)
            == selectedRectangleIndex) // 如果是选中的矩形
        {
            painter.fillRect(rect, selectedColor);
        }

        painter.drawRect(rect);
    }
}

void RectangleWidget::mousePressEvent(QMouseEvent *event)
{
    // 计算点击的矩形索引，由下往上从1开始
    int index = numRectangles
                - (event->position().y() - figureHeight / 20) / rectHeight;
    index = std::max(0, std::min(index, numRectangles));

    setSelectedRectangleIndex(index);

    // 输出点击的矩形索引
    emit rectangleClicked(index);

    fillRectanglePressEvent(event);
}

void RectangleWidget::fillRectanglePressEvent(QMouseEvent *event)
{
    int index = (event->position().y() - figureHeight / 20) / rectHeight;
    index = std::max(0, std::min(index, numRectangles));

    int colorIndex = -1;
    auto it = std::find(
        fillIndices.begin(), fillIndices.end(), numRectangles - (index + 1));
    if (it != fillIndices.end()) // 如果是填充颜色的矩形
    {
        colorIndex = std::distance(fillIndices.begin(), it);
    }

    emit fillRectangleClicked(colorIndex);
}