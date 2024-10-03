#ifndef RECTANGLE_WIDGET_H
#define RECTANGLE_WIDGET_H

#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QWidget>
#include <vector>

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
        // update(); // 重绘
    }

    // 设置需要填充的矩形及颜色
    void setFillRectangles(const std::vector<int> &indices,
                           QColor color = Qt::red)
    {
        fillIndices = indices;
        fillColor = color;
        // update();
    }

    // 设置需要修改下边界的矩形及颜色
    void setBorders(const std::vector<int> &indices, QColor color = Qt::blue)
    {
        borderIndices = indices;
        borderColor = color;
        // update();
    }

    // 设置选中的矩形索引
    void setSelectedRectangleIndex(int index, QColor color = Qt::green)
    {
        selectedRectangleIndex = index;
        selectedColor = color;
        update();
    }

protected:
    // 绘制矩形
    void paintEvent(QPaintEvent *) override;

    // 处理点击事件
    void mousePressEvent(QMouseEvent *event) override;

    // 处理被填充区域点击时的事件
    void fillRectanglePressEvent(QMouseEvent *event);

signals:
    void rectangleClicked(int index);
    void fillRectangleClicked(int index);

private:
    // 矩形数量
    int numRectangles{};
    // 每个矩形的尺寸
    int rectHeight{}, rectWidth{};
    // 整个图像区域的尺寸
    int figureHeight{}, figureWidth{};
    std::vector<int> fillIndices{}; // 记录需要填充颜色的矩形索引
    std::vector<int> borderIndices{}; // 记录需要修改下边界颜色的矩形索引
    int selectedRectangleIndex{-1};  // 选中的矩形索引
    QColor fillColor{Qt::red};       // 填充颜色
    QColor borderColor{Qt::blue};    // 下边界颜色
    QColor selectedColor{Qt::green}; // 选中颜色
};

#endif // !RECTANGLE_WIDGET_H
