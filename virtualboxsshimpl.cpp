#include "virtualboxsshimpl.h"

#include <QProcess>
#include <QTranslator>

#include <QDebug>

VirtualBoxSSHImpl* VirtualBoxSSHImpl::instance(){
     static VirtualBoxSSHImpl virtualBox;
     return &virtualBox;
}

VirtualBoxSSHImpl::VirtualBoxSSHImpl() :
    VirtualMachineInterface()
{
    qDebug() << "Konstructor VirtualBoxImpl";
}


QString VirtualBoxSSHImpl::name() const {
    return QString("VirtualBox");
}


QString VirtualBoxSSHImpl::description() const {
    return QString("Virtualbox Interface");
}


QList<QByteArray>  VirtualBoxSSHImpl::listVmUUIDs() {

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

QByteArray VirtualBoxSSHImpl::removeSurroundingChar(QByteArray string, const char removeChar){

    string = string.mid( string.indexOf(removeChar) + 1 );

    string.chop(1);

    return string;
}

///
// @return uuid, name, ostype, state, memory, cpumax
QHash<QByteArray, QByteArray>  VirtualBoxSSHImpl::listVmInfo( QByteArray id ) {

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

        if ( key.operator==("cpuexecutioncap") ){
            infoHash["cpumax"] = infoPair.last();
            continue;
        }

    }

    return infoHash;
}



bool VirtualBoxSSHImpl::startVm(int machine) const{
    return false;
}


QList<QByteArray> VirtualBoxSSHImpl::vBoxManageProcess( QStringList param ) const {
    QProcess vBoxProcess;

    param.prepend(QString("VBoxManage"));
    QByteArray vBoxManageCommand = param.join(" ").toLatin1();

    // Needs Keypair and the public key in .ssh/authorized_keys
    QStringList sshParamList = QStringList() << m_login + "@" + m_hostname;

    vBoxProcess.start("ssh", sshParamList );

    vBoxProcess.waitForStarted();

    // now that we are on the host, execute the vmBoxManage
    vBoxProcess.write(vBoxManageCommand);
    vBoxProcess.closeWriteChannel();

    vBoxProcess.waitForFinished();

    return vBoxProcess.readAll().split('\n');

}
