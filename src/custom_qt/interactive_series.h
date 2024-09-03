// interactive_series.h
#ifndef INTERACTIVE_SERIES_H
#define INTERACTIVE_SERIES_H

#include <QtCharts/QLineSeries>

class IaLineSeries : public QObject
{
    Q_OBJECT

public:
    explicit IaLineSeries(QLineSeries *series);

    QLineSeries *qlineseries;

private slots:
    void on_hovered(const QPointF &point, bool state);
};

#endif // INTERACTIVE_SERIES_H
