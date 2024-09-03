#include "interactive_chartview.h"

#include <QtCharts/QChartView>
#include <QtWidgets/QMenu>

IaChartView::IaChartView(QChartView *chartview) : qchartview(chartview)
{
    qchartview->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(qchartview,
            &QChartView::customContextMenuRequested,
            this,
            &IaChartView::show_context_menu);
}

void IaChartView::show_context_menu(const QPoint &pos)
{
    QMenu contextMenu(qchartview);
    QAction action1("Action 1", qchartview);
    QAction action2("Action 2", qchartview);

    connect(&action1, &QAction::triggered, this, []() {
        // 处理Action 1
    });

    connect(&action2, &QAction::triggered, this, []() {
        // 处理Action 2
    });

    contextMenu.addAction(&action1);
    contextMenu.addAction(&action2);
    contextMenu.exec(qchartview->mapToGlobal(pos));
}