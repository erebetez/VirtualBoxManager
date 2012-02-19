#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "VirtualMachineInterface.h"

#include <QSettings>
#include <QPluginLoader>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadSettings();

    m_settingsDialog = new SettingsDialog(this);
    m_settingsDialog->setModal(true);

    QString settingsPath = m_settings->fileName();
    settingsPath = settingsPath.left(settingsPath.length() - QString("VBoxManagerSettings.ini").length() );

    QString databaseFielName = m_settings->value("databaseFileName", QString("VBoxManagerDatabase.db")).toString();

    m_starter = new VmStarter(this);
    m_starter->connectToDatabase(settingsPath + databaseFielName);


    setUpUI();

    loadPlugins();

}

MainWindow::~MainWindow()
{
    m_settings->beginGroup("MainWindow");
    m_settings->setValue("size", size());
    m_settings->endGroup();

    delete m_settingsDialog;
    delete ui;
    delete m_starter;
    delete m_settings;
    qDeleteAll( m_hypervisorList );
}

void MainWindow::loadSettings(){

    m_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope , "VBoxManager", "VBoxManagerSettings");

    m_settings->beginGroup("MainWindow");
    resize(m_settings->value("size", QSize(600, 400)).toSize());

    // Needs to write a value and sync it in order to create the folder the first time the program is started.
    // Database will not open otherwiese.
    m_settings->setValue("size", size());

    m_settings->endGroup();
    m_settings->sync();

}


void MainWindow::loadPlugins()
{

    foreach (QObject *plugin, QPluginLoader::staticInstances()){
        qDebug() << "static: " << plugin->metaObject()->className();

        VirtualMachineInterface *iVBox = qobject_cast<VirtualMachineInterface *>(plugin);
        qDebug() << "desc: " << iVBox->description();

    }

    // TODO: Make it also work for dynamic plugins
//    pluginsDir = QDir(qApp->applicationDirPath());

//#if defined(Q_OS_WIN)
//    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
//        pluginsDir.cdUp();
//#elif defined(Q_OS_MAC)
//    if (pluginsDir.dirName() == "MacOS") {
//        pluginsDir.cdUp();
//        pluginsDir.cdUp();
//        pluginsDir.cdUp();
//    }
//#endif
//    pluginsDir.cd("plugins");

//    qDebug() << pluginsDir;

//    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
//        qDebug() << fileName;
//        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
//        QObject *plugin = loader.instance();
//        if (plugin) {
//            qDebug() << "loaded: " << plugin->metaObject()->className();
//            m_pluginFileNames += fileName;
//        }
//    }
}




void MainWindow::setUpUI()
{
    setDockNestingEnabled(true);

    ui->mainToolBar->addAction(tr("Populate"), this, SLOT(populateDataBase()) );
    ui->mainToolBar->addAction(tr("Copy"), this, SLOT(copyVm()) );
    ui->mainToolBar->addAction(tr("Settings"), this, SLOT(showSettings()) );

    m_dockList = new ListDialog( this );
    m_dockList->setFeatures( QDockWidget::AllDockWidgetFeatures );

    addDockWidget( Qt::BottomDockWidgetArea, m_dockList, Qt::Vertical );

    connect( m_starter, SIGNAL(dbRefreshed()), m_dockList, SLOT(update()));
}

void MainWindow::populateDataBase(){
    m_starter->populateDb(m_hypervisorList);
}

void MainWindow::copyVm(){
    QByteArray machine = m_dockList->currentMachine();

    if( machine.isEmpty() ) {
        return;
    }
//    VirtualBoxSshPlugin::instance()->copyVm(machine, QByteArray("MyClone"));

}

void MainWindow::showSettings(){

    m_settingsDialog->setHypervisors(m_hypervisorList);
    m_settingsDialog->show();

}
