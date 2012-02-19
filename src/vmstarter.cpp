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

    initDatabase();

    foreach(Hypervisor* hy, hypervisorList){
        qDebug() << "checkingout: " << hy->name();

        foreach (QObject *plugin, QPluginLoader::staticInstances()){
            VirtualMachineInterface *iVMachine = qobject_cast<VirtualMachineInterface *>(plugin);

            if(hy->typ() == iVMachine->name()){
                iVMachine->setHostName(hy->adress());
                iVMachine->setLoginName(hy->user());

                QList<QByteArray> virtualMachineList = iVMachine->listVmUUIDs();

                foreach(QByteArray vitualMachine, virtualMachineList){
                    QHash<QByteArray, QString> vitualMachineInfo = iVMachine->listVmInfo(vitualMachine);

                    qDebug() << "Infos for VM:" << vitualMachineInfo.value("name");

                    QSqlQuery query;

                    query.prepare("INSERT INTO virtualmachines (uuid, name, host, hypervisor, ostype, state, memory, cpumax) "
                                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");

                    query.addBindValue( vitualMachineInfo.value("UUID") );
                    query.addBindValue( vitualMachineInfo.value("name") );
                    query.addBindValue( hy->adress() );
                    query.addBindValue( iVMachine->name() );
                    query.addBindValue( vitualMachineInfo.value("ostype") );
                    query.addBindValue( vitualMachineInfo.value("state") );
                    query.addBindValue( vitualMachineInfo.value("memory") );
                    query.addBindValue( vitualMachineInfo.value("cpumax") );
                    query.exec();

//                    qDebug() << query.executedQuery();
                    qDebug() << "Insert" << query.lastError();

                    // Machine already exixts
                    if(query.lastError().number() == 19){

                        QSqlQuery query2;

                        query2.prepare("UPDATE virtualmachines "
                                       "SET name = ?, host = ?, hypervisor = ?, ostype = ?, state = ?, memory = ?, cpumax = ? "
                                       "WHERE uuid = '" + vitualMachineInfo.value("UUID") + "'");

                        query2.addBindValue( vitualMachineInfo.value("name"));
                        query2.addBindValue( hy->adress() );
                        query2.addBindValue( iVMachine->name() );
                        query2.addBindValue( vitualMachineInfo.value("ostype") );
                        query2.addBindValue( vitualMachineInfo.value("state") );
                        query2.addBindValue( vitualMachineInfo.value("memory") );
                        query2.addBindValue( vitualMachineInfo.value("cpumax") );
                        query2.exec();

//                        qDebug() << query2.executedQuery();
                        qDebug() << "Update" << query2.lastError();
                    }
                }

            }
        }
    }
    emit dbRefreshed();
}



VmStarter::~VmStarter(){
}


