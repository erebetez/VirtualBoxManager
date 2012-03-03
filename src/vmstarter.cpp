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



void VmStarter::setHypervisorList(const QList<Hypervisor *> list){
   m_hypervisorList = list;
}


void VmStarter::populateVmsDb(){

    QSqlQuery query("UPDATE virtualmachines SET available = 'F'");

    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO virtualmachines (vmuuid, name, hypervisor, ostype, state, memorymax, cpumax, available) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");

    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE virtualmachines "
                   "SET name = ?, hypervisor = ?, ostype = ?, state = ?, memorymax = ?, cpumax = ? , available = ?"
                   "WHERE vmuuid = ?");

    VirtualMachineInterface *iVMachine;

    foreach(Hypervisor* hy, m_hypervisorList){
        qDebug() << "checkingout: " << hy->name();

        iVMachine = vmInstance(hy);

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

void VmStarter::start(const QByteArray id){

    QByteArray hypervisorName = getHypervisorForMachineId(id);
    VirtualMachineInterface *iVMachine;

    qDebug() << "looking for" << hypervisorName;

    foreach(Hypervisor* hy, m_hypervisorList){
        if( hy->name() == hypervisorName ){
            iVMachine = vmInstance(hy);
            iVMachine->startVm(id);
        }
    }
}

void VmStarter::stop(const QByteArray id){

    QByteArray hypervisorName = getHypervisorForMachineId(id);
    VirtualMachineInterface *iVMachine;

    qDebug() << "looking for" << hypervisorName;

    foreach(Hypervisor* hy, m_hypervisorList){
        if( hy->name() == hypervisorName ){
            iVMachine = vmInstance(hy);
            iVMachine->stopVm(id);
        }
    }
}


void VmStarter::copy(const QByteArray id){

}

VirtualMachineInterface* VmStarter::vmInstance(Hypervisor *hy) const {
    foreach (QObject *plugin, QPluginLoader::staticInstances()){
        VirtualMachineInterface *iVMachine = qobject_cast<VirtualMachineInterface *>(plugin);

        if(hy->typ() == iVMachine->name()){
            iVMachine->setHostName(hy->adress());
            iVMachine->setLoginName(hy->user());
            return iVMachine;
        }
    }
    return 0;
}


QByteArray VmStarter::getHypervisorForMachineId(const QByteArray id) const {
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


