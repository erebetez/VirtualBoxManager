#include "vmstarter.h"

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

void VmStarter::initDatabase()
{
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

    qDebug() << query.lastError();
}


void VmStarter::populateDb(){

    // Currently there is only the SSH impl
    m_vmInstance = VirtualBoxSSHImpl::instance();


    m_vmInstance->setHostName("localhost");
    m_vmInstance->setLoginName("etienne");


    QList<QByteArray> virtualMachineList = m_vmInstance->listVmUUIDs();

    qDebug() << virtualMachineList;

    foreach(QByteArray vitualMachine, virtualMachineList){
        QHash<QByteArray, QByteArray> vitualMachineInfo = VirtualBoxSSHImpl::instance()->listVmInfo(vitualMachine);

        qDebug() << vitualMachineInfo["name"];

        QSqlQuery query;
        query.prepare("INSERT INTO virtualmachines (uuid, name, host, ostype, state, memory, cpumax) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?)");

        query.bindValue(0, vitualMachineInfo["UUID"]);
        query.bindValue(1, vitualMachineInfo["name"]);
        query.bindValue(2, "localhost");
        query.bindValue(3, vitualMachineInfo["ostype"]);
        query.bindValue(4, vitualMachineInfo["state"]);
        query.bindValue(5, vitualMachineInfo["memory"]);
        query.bindValue(6, vitualMachineInfo["cpumax"]);
        query.exec();
    }

    emit dbRefreshed();
}


VmStarter::~VmStarter(){
}


