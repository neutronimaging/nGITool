#include "ngimainwindow.h"
#include "ui_ngimainwindow.h"

nGIMainWindow::nGIMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::nGIMainWindow)
{
    ui->setupUi(this);
}

nGIMainWindow::~nGIMainWindow()
{
    delete ui;
}
