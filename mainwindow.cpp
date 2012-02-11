#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vmstarter.h"


#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    VmStarter *starter = new VmStarter(this);

    ui->centralMainLayout->layout()->addWidget(starter);

    ui->mainToolBar->addAction(tr("Settings"), this, SLOT(showSettings()) );

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::showSettings(){
    qDebug() << "showSettings";

    if( m_settings) {
        m_settings = new SettingsDialog(this);
    }
    m_settings->show();



}
