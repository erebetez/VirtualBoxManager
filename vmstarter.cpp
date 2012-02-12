#include "vmstarter.h"
#include "virtualboxsshimpl.h"

#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QTreeWidget>
#include <QHash>
#include <QtSql>


#include <QDebug>


VmStarter::VmStarter(QObject *parent)
    : QObject(parent)
{
}

bool VmStarter::connectToDatabase(const QString &dataBaseFileName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(dataBaseFileName);

    if (!db.open()) {
        qDebug() << db.lastError();
        return false;
    }

    initDatabase();

    return true;
}

void VmStarter::clearDatabase() const {
    QSqlQuery query;
    query.exec("DROP TABLE virtualmachines");

}

void VmStarter::initDatabase() const {
    QSqlQuery query;
    query.exec("create table IF NOT EXISTS virtualmachines (uuid PRIMARY KEY, "
               "name, "
               "host, "
               "ostype, "
               "state, "
               "memory, "
               "cpumax "
               ")"
              );

//    qDebug() << query.lastError();
}


void VmStarter::populateDb(){
    // reset the hole stuff
    clearDatabase();
    initDatabase();

    // Currently there is only the SSH impl
    m_vmInstance = VirtualBoxSSHImpl::instance();


    m_vmInstance->setHostName("localhost");
    m_vmInstance->setLoginName("etienne");

    QList<QByteArray> virtualMachineList = m_vmInstance->listVmUUIDs();

//    qDebug() << virtualMachineList;

    foreach(QByteArray vitualMachine, virtualMachineList){
        QHash<QByteArray, QByteArray> vitualMachineInfo = m_vmInstance->listVmInfo(vitualMachine);

        qDebug() << vitualMachineInfo.value("name");

        QSqlQuery query;

//        query.exec("SELECT UUID FROM virtualmachines");
//        qDebug() << query.first();
//        qDebug() << query.value(0);

//        "UPDATE table_name"
//        "SET column1=value, column2=value2,..."
//        "WHERE some_column=some_value"

        query.prepare("INSERT INTO virtualmachines (uuid, name, host, ostype, state, memory, cpumax) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?)");

        query.bindValue(0, vitualMachineInfo.value("UUID"));
        query.bindValue(1, vitualMachineInfo.value("name"));
        query.bindValue(2, "localhost");
        query.bindValue(3, vitualMachineInfo.value("ostype"));
        query.bindValue(4, vitualMachineInfo.value("state"));
        query.bindValue(5, vitualMachineInfo.value("memory"));
        query.bindValue(6, vitualMachineInfo.value("cpumax"));
        query.exec();
    }

    emit dbRefreshed();
}



VmStarter::~VmStarter(){
}


