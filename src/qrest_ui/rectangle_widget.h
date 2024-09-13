#ifndef RECTANGLE_WIDGET_H
#define RECTANGLE_WIDGET_H

#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QWidget>


class RectangleWidget : public QWidget
{
    Q_OBJECT

public:
    RectangleWidget(QWidget *parent = nullptr, int num = 0)
        : QWidget(parent), numRectangles(num)
    {} // 默认 0 个矩形

    // 设置矩形数量
    void setNumRectangles(int num)
    {
        numRectangles = num;
        update(); // 重绘
    }

protected:
    // 绘制矩形
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);

        // 设置线宽为 5 像素
        QPen pen;
        pen.setWidth(5);         // 设置线宽
        pen.setColor(Qt::black); // 设置线条颜色
        painter.setPen(pen);

        // 获取窗口的宽高
        int widgetWidth = width();
        int widgetHeight = height();

        // 每个矩形的高度和宽度，左右预留1/4个宽度
        rectHeight = widgetHeight / (numRectangles == 0 ? 1 : numRectangles);
        rectWidth = widgetWidth * 2 / 3;

        // 绘制每个矩形
        for (int i = 0; i < numRectangles; ++i)
        {
            QRect rect(rectWidth / 4, i * rectHeight, rectWidth, rectHeight);
            painter.drawRect(rect);
        }
    }

    // 处理点击事件
    void mousePressEvent(QMouseEvent *event) override
    {
        // 计算点击的矩形索引，由下往上从1开始
        int index = numRectangles - (event->position().y() / rectHeight);

        // 输出点击的矩形索引
        emit rectangleClicked(index);
    }

signals:
    void rectangleClicked(int index);

private:
    int numRectangles;
    int rectHeight{};
    int rectWidth{};
};


#endif // !RECTANGLE_WIDGET_H
