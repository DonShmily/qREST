#include "main_window.h"

QtMainWindow::QtMainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow())
{
    ui->setupUi(this);
}

QtMainWindow::~QtMainWindow()
{
    if (chart != nullptr)
    {
        delete chart;
    }
    if (ui != nullptr)
    {
        delete ui;
    }
}
