#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vmstarter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    VmStarter *starter = new VmStarter(this);

    ui->centralMainLayout->layout()->addWidget(starter);

}

MainWindow::~MainWindow()
{
    delete ui;
}
