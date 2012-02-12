#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "virtualboxsshimpl.h"

#include <QSettings>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_starter = new VmStarter(this);


    QSettings settings(QSettings::IniFormat, QSettings::UserScope , "VBoxManager", "VBoxManagerSettings");

    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(600, 400)).toSize());

    // Needs to write a value and sync it in order to create the folder the first time the program is started.
    // Database will not open otherwiese.
    settings.setValue("size", size());

    settings.endGroup();
    settings.sync();

    QString settingsPath = settings.fileName();
    settingsPath = settingsPath.left(settingsPath.length() - QString("VBoxManagerSettings.ini").length() );


    QString databaseFielName = settings.value("databaseFileName", QString("VBoxManagerDatabase.db")).toString();

    m_starter->connectToDatabase(settingsPath + databaseFielName);


    setUpUI();
}

MainWindow::~MainWindow()
{

    QSettings settings(QSettings::IniFormat, QSettings::UserScope , "VBoxManager", "VBoxManagerSettings");
    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.endGroup();

    delete ui;
    delete m_starter;
}


void MainWindow::setUpUI()
{
    setDockNestingEnabled(true);

    ui->mainToolBar->addAction(tr("Populate"), m_starter, SLOT(populateDb()) );
    ui->mainToolBar->addAction(tr("Copy"), this, SLOT(copyVm()) );
    ui->mainToolBar->addAction(tr("Settings"), this, SLOT(showSettings()) );

    m_dockList = new ListDialog( this );
    m_dockList->setFeatures( QDockWidget::AllDockWidgetFeatures );

    addDockWidget( Qt::BottomDockWidgetArea, m_dockList, Qt::Vertical );

    connect( m_dockList, SIGNAL(dbRefreshed()), m_starter, SLOT(update()));
}

void MainWindow::copyVm(){
    VirtualBoxSSHImpl::instance()->copyVm(m_dockList->currentMachine());

}

void MainWindow::showSettings(){

    if( m_settings == 0) {
        qDebug() << "create Settings";
        m_settings = new SettingsDialog(this);
    }
    m_settings->show();

}
