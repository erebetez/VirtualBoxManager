#include "mainwindow.h"
#include "ui_mainwindow.h"



#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_starter = new VmStarter(this);

    setUpUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setUpUI()
{
    setDockNestingEnabled(true);

    ui->mainToolBar->addAction(tr("Populate"), m_starter, SLOT(populateDb()) );
    ui->mainToolBar->addAction(tr("Settings"), this, SLOT(showSettings()) );

    m_dockList = new ListDialog( this );
    m_dockList->setFeatures( QDockWidget::AllDockWidgetFeatures );

    addDockWidget( Qt::BottomDockWidgetArea, m_dockList, Qt::Vertical );


    connect( m_starter, SIGNAL(dbRefreshed()), m_dockList, SLOT(update()));
}


void MainWindow::showSettings(){

    if( m_settings == 0) {
        qDebug() << "create Settings";
        m_settings = new SettingsDialog(this);
    }
    m_settings->show();

}
