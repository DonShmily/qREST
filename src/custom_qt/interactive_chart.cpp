#include "interactive_chart.h"

#include <QtCharts/QChartView>
#include <QtWidgets/QMenu>

#include "interactive_series.h"

IaChart::IaChart(QChart *chart) : qchart(chart)
{
    // 创建一个 QLineSeries 对象
    QLineSeries *line_series = new QLineSeries();
    UpdateSeries(line_series);

    // 创建一个 IaLineSeries 对象
    IaLineSeries *interactive_series = new IaLineSeries(line_series);

    // 将 series 添加到 chart
    qchart->addSeries(interactive_series->qlineseries);
    qchart->createDefaultAxes();
}

void IaChart::UpdateSeries(QLineSeries *series)
{
    for (int i = 1; i != 50; ++i)
    {
        series->append(i, i * i);
    }
}