#include "vmstarter.h"

#include "VirtualMachineInterface.h"

#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QTreeWidget>
#include <QHash>
#include <QtSql>

#include <QPluginLoader>

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
               "hypervisor,"
               "ostype, "
               "state, "
               "memory, "
               "cpumax "
               ")"
              );

//    qDebug() << query.lastError();
}


void VmStarter::populateDb(QList<Hypervisor*> hypervisorList){
    // reset the hole stuff
    clearDatabase();
    initDatabase();

    foreach(Hypervisor* hy, hypervisorList){
        qDebug() << "checkingout: " << hy->name();

        foreach (QObject *plugin, QPluginLoader::staticInstances()){
            VirtualMachineInterface *iVMachine = qobject_cast<VirtualMachineInterface *>(plugin);

            if(hy->typ() == iVMachine->name()){
                iVMachine->setHostName(hy->adress());
                iVMachine->setLoginName(hy->user());

                QList<QByteArray> virtualMachineList = iVMachine->listVmUUIDs();

            //    qDebug() << virtualMachineList;

                foreach(QByteArray vitualMachine, virtualMachineList){
                    QHash<QByteArray, QByteArray> vitualMachineInfo = iVMachine->listVmInfo(vitualMachine);

                    qDebug() << "Infos for VM:" << vitualMachineInfo.value("name");

                    QSqlQuery query;

            //        query.exec("SELECT UUID FROM virtualmachines");
            //        qDebug() << query.first();
            //        qDebug() << query.value(0);

            //        "UPDATE table_name"
            //        "SET column1=value, column2=value2,..."
            //        "WHERE some_column=some_value"

                    query.prepare("INSERT INTO virtualmachines (uuid, name, host, hypervisor, ostype, state, memory, cpumax) "
                                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");

                    query.bindValue(0, vitualMachineInfo.value("UUID"));
                    query.bindValue(1, vitualMachineInfo.value("name"));
                    query.bindValue(2, hy->adress());
                    query.bindValue(3, iVMachine->description());
                    query.bindValue(4, vitualMachineInfo.value("ostype"));
                    query.bindValue(5, vitualMachineInfo.value("state"));
                    query.bindValue(6, vitualMachineInfo.value("memory"));
                    query.bindValue(7, vitualMachineInfo.value("cpumax"));
                    query.exec();
                    qDebug() << query.lastError();
                }

            }
        }
    }
    emit dbRefreshed();
}



VmStarter::~VmStarter(){
}


