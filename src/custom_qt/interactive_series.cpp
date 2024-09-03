// interactive_series.cpp
#include "interactive_series.h"

IaLineSeries::IaLineSeries(QLineSeries *series) : qlineseries(series)
{
    connect(
        qlineseries, &QLineSeries::hovered, this, &IaLineSeries::on_hovered);
}

void IaLineSeries::on_hovered(const QPointF &point, bool state)
{
    Q_UNUSED(point);
    QPen pen = qlineseries->pen();
    pen.setWidth(state ? pen.width() + 2 : pen.width() - 2);
    qlineseries->setPen(pen);
}
