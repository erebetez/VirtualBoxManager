#include "virtualboximpl.h"

#include <QProcess>
#include <QTranslator>

#include <QDebug>

VirtualBoxImpl* VirtualBoxImpl::instance(){
     static VirtualBoxImpl virtualBox;
     return &virtualBox;
}

VirtualBoxImpl::VirtualBoxImpl() :
    VirtualMachineInterface()
{
    qDebug() << "Konstructor VirtualBoxImpl";
}



QString VirtualBoxImpl::name() const {
    return QString("VirtualBox");
}


QString VirtualBoxImpl::description() const {
    return QString("Virtualbox Interface");
}


QList<QByteArray>  VirtualBoxImpl::listVmUUIDs() {

    QList<QByteArray> vmList = vBoxManageProcess( QStringList() << "list" << "vms" );

    QByteArray vmRow;

    QList<QByteArray> newList;

    for( int i = 0; i < vmList.length(); ++i ){
        vmRow = vmList.at(i);

        vmRow = removeSurroundingChar(vmRow, '{');

        if( vmRow.isEmpty() ){
            continue;
        }

        newList.append(vmRow);
    }

    return newList;
}

QByteArray VirtualBoxImpl::removeSurroundingChar(QByteArray string, const char removeChar){

    string = string.mid( string.indexOf(removeChar) + 1 );

    string.chop(1);

    return string;
}

///
// @return uuid, name, ostype, state, memory, cpumax
QHash<QByteArray, QByteArray>  VirtualBoxImpl::listVmInfo( QByteArray id ) {

    QList<QByteArray> vmInfos = vBoxManageProcess( QStringList() << "showvminfo" << id << "--details" << "--machinereadable" );

    QHash<QByteArray, QByteArray> infoHash;

    foreach(QByteArray vmInfo, vmInfos){
        QList<QByteArray> infoPair = vmInfo.split('=');

        QString key = QString(infoPair.takeFirst());

        if ( key.operator==("UUID") ){
            infoHash["UUID"] = removeSurroundingChar(infoPair.last(), '"');
            continue;
        }

        if ( key.operator==("name") ){
            infoHash["name"] = removeSurroundingChar(infoPair.last(), '"');
            continue;
        }

        if ( key.operator==("ostype") ){
            infoHash["ostype"] = removeSurroundingChar(infoPair.last(), '"');
            continue;
        }

        if ( key.operator==("VMState") ){
            infoHash["state"] = removeSurroundingChar(infoPair.last(), '"');
            continue;
        }

        if ( key.operator==("memory") ){
            infoHash["memory"] = infoPair.last();
            continue;
        }

        if ( key.operator==("cpuexecuteioncap") ){
            infoHash["cpumax"] = infoPair.last();
            continue;
        }

    }

    return infoHash;
}



bool VirtualBoxImpl::startVm(int machine) const{
    return false;
}


QList<QByteArray> VirtualBoxImpl::vBoxManageProcess( const QStringList param ) const {
    QProcess vBoxProcess;

    vBoxProcess.start("VBoxManage", param );

    vBoxProcess.waitForFinished();

    return vBoxProcess.readAll().split('\n');

}
