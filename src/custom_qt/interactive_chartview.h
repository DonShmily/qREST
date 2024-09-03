#ifndef INTERACTIVE_CHART_VIEW_H
#define INTERACTIVE_CHART_VIEW_H

#include <QtCharts/QChartView>

class IaChartView : public QObject
{
    Q_OBJECT

public:
    explicit IaChartView(QChartView *chart_view);

    QChartView *qchartview;

private slots:
    void show_context_menu(const QPoint &pos);

private:
};

#endif // ! INTERACTIVE_CHART_VIEW_H