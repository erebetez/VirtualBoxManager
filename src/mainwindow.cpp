#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>
#include <QPluginLoader>
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

    loadPlugins();

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

    connect( m_starter, SIGNAL(dbRefreshed()), m_dockList, SLOT(update()));
}

void MainWindow::copyVm(){
    QByteArray machine = m_dockList->currentMachine();

    if( machine.isEmpty() ) {
        return;
    }
//    VirtualBoxSshPlugin::instance()->copyVm(machine, QByteArray("MyClone"));

}

void MainWindow::showSettings(){

    if( m_settings == 0) {
        qDebug() << "create Settings";
        m_settings = new SettingsDialog(this);
    }
    m_settings->show();

}
