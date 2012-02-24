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
    query.exec("create table IF NOT EXISTS virtualmachines (vmuuid PRIMARY KEY, "
               "name, "
               "hypervisor, "
               "host, "
               "ostype, "
               "state, "
               "memorymax, "
               "cpumax, "
               "deleted"
               ")"
              );

    query.exec("create table IF NOT EXISTS harddiscs (hduuid PRIMARY KEY, "
               "vmuuid, "
               "location, "
               "maxsize, "
               "currentsize, "
               "deleted"
               ")"
              );

    query.exec("create table IF NOT EXISTS hdused (vmuuid, "
               "hduuid "
               ")"
              );

//    qDebug() << query.lastError();
}


void VmStarter::populateDb(QList<Hypervisor*> hypervisorList){

    QSqlQuery query;

    query.exec("UPDATE virtualmachines SET deleted = 'T'");

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


                    query.prepare("INSERT INTO virtualmachines (vmuuid, name, host, hypervisor, ostype, state, memorymax, cpumax, deleted) "
                                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");

                    query.addBindValue( vitualMachineInfo.value("UUID") );
                    query.addBindValue( vitualMachineInfo.value("name") );
                    query.addBindValue( hy->adress() );
                    query.addBindValue( iVMachine->name() );
                    query.addBindValue( vitualMachineInfo.value("ostype") );
                    query.addBindValue( vitualMachineInfo.value("state") );
                    query.addBindValue( vitualMachineInfo.value("memory") );
                    query.addBindValue( vitualMachineInfo.value("cpumax") );
                    query.addBindValue( "F" );
                    query.exec();

//                    qDebug() << query.executedQuery();
                    qDebug() << "Insert" << query.lastError();

                    // Machine already exixts
                    if(query.lastError().number() == 19){

                        query.prepare("UPDATE virtualmachines "
                                       "SET name = ?, host = ?, hypervisor = ?, ostype = ?, state = ?, memorymax = ?, cpumax = ? , deleted = ?"
                                       "WHERE vmuuid = '" + vitualMachineInfo.value("UUID") + "'");

                        query.addBindValue( vitualMachineInfo.value("name"));
                        query.addBindValue( hy->adress() );
                        query.addBindValue( iVMachine->name() );
                        query.addBindValue( vitualMachineInfo.value("ostype") );
                        query.addBindValue( vitualMachineInfo.value("state") );
                        query.addBindValue( vitualMachineInfo.value("memory") );
                        query.addBindValue( vitualMachineInfo.value("cpumax") );
                        query.addBindValue( "F" );
                        query.exec();

//                        qDebug() << query2.executedQuery();
                        qDebug() << "Update" << query.lastError();
                    }
                }

            }
        }
    }
    emit dbRefreshed();
}


QByteArray VmStarter::getHypervisorForMachineId(const QByteArray id) {
   QSqlQuery query;
   // TODO I actually need the hypervisor name, given in the settings.
   query.prepare("SELECT hypervisor FROM virtualmachines WHERE vmuuid like ?");
   query.addBindValue(id);
   query.exec();
   query.first();
   return query.record().value(0).toByteArray();
}

VmStarter::~VmStarter(){
}


