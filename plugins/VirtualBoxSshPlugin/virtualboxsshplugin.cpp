#include "virtualboxsshplugin.h"

#include <QProcess>
#include <QTranslator>

#include <QDebug>


QByteArray VirtualBoxSshPlugin::name() const {
    return QByteArray("VirtualBox");
}


QString VirtualBoxSshPlugin::description() const {
    return QString("Virtualbox SSH Interface");
}

QString VirtualBoxSshPlugin::info() const{
    return tr("Executes vboxmanager over an ssh connection.");
}


QList<QByteArray>  VirtualBoxSshPlugin::listVmUUIDs() {

    QList<QByteArray> vmList = vBoxManageProcess( "list vms" );

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

QByteArray VirtualBoxSshPlugin::removeSurroundingChar(QByteArray string, const char removeChar){

    string = string.mid( string.indexOf(removeChar) + 1 );

    string.chop(1);

    return string;
}

///
// @return uuid, name, ostype, state, memory, cpumax
QHash<QByteArray, QString> VirtualBoxSshPlugin::listVmInfo( QByteArray id ) {

    QList<QByteArray> vmInfos = vBoxManageProcess( QByteArray("showvminfo " + id + " --details --machinereadable") );

    QHash<QByteArray, QString> infoHash;

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


// Harddisc TODO





bool VirtualBoxSshPlugin::startVm( const QByteArray id ) const{
    executeInSSHShell("startvm " + id);
    return false;
}


QList<QByteArray> VirtualBoxSshPlugin::vBoxManageProcess( QByteArray command ) const {

    QByteArray returnValue = executeInSSHShell(command);

    return returnValue.split('\n');
}


QByteArray VirtualBoxSshPlugin::executeInSSHShell(QByteArray command) const {
    QProcess vBoxProcess;

    // Needs Keypair and the public key in .ssh/authorized_keys
    QStringList sshParamList = QStringList() << m_login + "@" + m_hostname;

//    qDebug() << "logindata:" << m_login << m_hostname;

    vBoxProcess.start("ssh", sshParamList );

    vBoxProcess.waitForStarted();

    // now that we are on the host, execute the vmBoxManage
    command.prepend("VBoxManage ");

    //qDebug() << "ssh Command: " << command;

    vBoxProcess.write(command);
    vBoxProcess.closeWriteChannel();

    vBoxProcess.waitForFinished();

    return vBoxProcess.readAll();
}

bool VirtualBoxSshPlugin::copyVm( const QByteArray id, const QByteArray name ) const{
    QByteArray cloneVm = QByteArray("clonevm " + id + " --register --name " + name);

    QByteArray returnValue = executeInSSHShell(cloneVm);

    qDebug() << returnValue;

    return false;
}

Q_EXPORT_PLUGIN2(virtualboxsshplugin, VirtualBoxSshPlugin)
