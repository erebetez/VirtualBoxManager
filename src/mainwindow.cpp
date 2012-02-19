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

    QCoreApplication::setOrganizationName("VMachineManager");
    QCoreApplication::setApplicationName("VMachineManagerSettings");

    m_settings = new Settings(this);
    loadSettings();

    m_starter = new VmStarter(this);
    m_starter->connectToDatabase(m_settings->databasePath());

    setUpUI();

    loadPlugins();

}

MainWindow::~MainWindow()
{
    m_settings->beginGroup("MainWindow");
    m_settings->setValue("size", size());
    m_settings->endGroup();

    delete m_settings;
    delete ui;
    delete m_starter;
}

void MainWindow::loadSettings(){

    m_settings->beginGroup("MainWindow");
    resize(m_settings->value("size", QSize(600, 400)).toSize());
    m_settings->endGroup();


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
    ui->mainToolBar->addAction(tr("Settings"), m_settings, SLOT(showDialog()) );

    m_dockList = new ListDialog( this );
    m_dockList->setFeatures( QDockWidget::AllDockWidgetFeatures );

    addDockWidget( Qt::BottomDockWidgetArea, m_dockList, Qt::Vertical );

    connect( m_starter, SIGNAL(dbRefreshed()), m_dockList, SLOT(update()));
}

void MainWindow::populateDataBase(){
    m_starter->populateDb(m_settings->hypervisors());
}

void MainWindow::copyVm(){
    QByteArray machine = m_dockList->currentMachine();

    if( machine.isEmpty() ) {
        return;
    }
//    VirtualBoxSshPlugin::instance()->copyVm(machine, QByteArray("MyClone"));

}

