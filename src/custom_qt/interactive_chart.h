#ifndef INTERACTIVE_CHART_H
#define INTERACTIVE_CHART_H

#include <QChart>
#include <QtCharts/QLineSeries>

class IaChart : public QObject
{
    Q_OBJECT

public:
    explicit IaChart(QChart *chart);

    QChart *qchart;

private slots:

private:
    void UpdateSeries(QLineSeries *series);
};

#endif // INTERACTIVE_CHART_H