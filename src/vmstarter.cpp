#include "vmstarter.h"



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
               "ostype, "
               "state, "
               "memorymax, "
               "cpumax, "
               "available"
               ")"
              );

    query.exec("create table IF NOT EXISTS harddiscs (hduuid PRIMARY KEY, "
               "vmuuid, "
               "location, "
               "maxsize, "
               "currentsize, "
               "available"
               ")"
              );

    query.exec("create table IF NOT EXISTS hdused (vmuuid, "
               "hduuid "
               ")"
              );

    query.exec("create table IF NOT EXISTS hypervisors (hypervisor PRIMARY KEY, "
               "host, "
               "typ, "
               "user  "
               ")"
              );

//    qDebug() << query.lastError();
}


void VmStarter::populateDb(QList<Hypervisor*> hypervisorList){

    QSqlQuery query("UPDATE virtualmachines SET available = 'F'");

    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO virtualmachines (vmuuid, name, hypervisor, ostype, state, memorymax, cpumax, available) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");

    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE virtualmachines "
                   "SET name = ?, hypervisor = ?, ostype = ?, state = ?, memorymax = ?, cpumax = ? , available = ?"
                   "WHERE vmuuid = ?");

    VirtualMachineInterface *iVMachine;

    foreach(Hypervisor* hy, hypervisorList){
        qDebug() << "checkingout: " << hy->name();

        iVMachine = vmInstance(hy->typ());
        iVMachine->setHostName(hy->adress());
        iVMachine->setLoginName(hy->user());

        QList<QByteArray> virtualMachineList = iVMachine->listVmUUIDs();

        foreach(QByteArray vitualMachine, virtualMachineList){
            QHash<QByteArray, QString> vitualMachineInfo = iVMachine->listVmInfo(vitualMachine);

            qDebug() << "Infos for VM:" << vitualMachineInfo.value("name");

            insertQuery.addBindValue( vitualMachineInfo.value("UUID") );
            insertQuery.addBindValue( vitualMachineInfo.value("name") );
            insertQuery.addBindValue( hy->name() );
            insertQuery.addBindValue( vitualMachineInfo.value("ostype") );
            insertQuery.addBindValue( vitualMachineInfo.value("state") );
            insertQuery.addBindValue( vitualMachineInfo.value("memory") );
            insertQuery.addBindValue( vitualMachineInfo.value("cpumax") );
            insertQuery.addBindValue( "T" );
            insertQuery.exec();

//          qDebug() << query.executedQuery();
            qDebug() << "Insert" << insertQuery.lastError();

            // Machine already exixts
            if(insertQuery.lastError().number() == 19){
                updateQuery.addBindValue( vitualMachineInfo.value("name"));
                updateQuery.addBindValue( hy->name() );
                updateQuery.addBindValue( vitualMachineInfo.value("ostype") );
                updateQuery.addBindValue( vitualMachineInfo.value("state") );
                updateQuery.addBindValue( vitualMachineInfo.value("memory") );
                updateQuery.addBindValue( vitualMachineInfo.value("cpumax") );
                updateQuery.addBindValue( "T" );
                updateQuery.addBindValue( vitualMachineInfo.value("UUID") );
                updateQuery.exec();

//              qDebug() << query2.executedQuery();
                qDebug() << "Update" << updateQuery.lastError();
            }
        }

    }

    emit dbRefreshed();
}


VirtualMachineInterface* VmStarter::vmInstance(QByteArray typ) const {
    foreach (QObject *plugin, QPluginLoader::staticInstances()){
        VirtualMachineInterface *iVMachine = qobject_cast<VirtualMachineInterface *>(plugin);

        if(typ == iVMachine->name()){
            return iVMachine;
        }
    }
    return 0;
}


QByteArray VmStarter::getHypervisorForMachineId(const QByteArray id) {
   QSqlQuery query;
   query.prepare("SELECT h.hypervisor FROM hypervisors h INNER JOIN virtualmachines m ON h.hypervisor = m.hypervisor WHERE m.vmuuid like ?");
   query.addBindValue(id);
   query.exec();
   query.first();
   qDebug() << query.lastError();
   return query.record().value(0).toByteArray();
}

VmStarter::~VmStarter(){
}


