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

    QList<QByteArray> vmList = vBoxManageProcess( "list vms -l" );

    QString vmRow;

    m_vmHashList.clear();
    m_vmsUuidList.clear();

    for( int i = 0; i < vmList.length(); ++i ){
        vmRow = QString(vmList.at(i).simplified());

        qDebug() << i << ":" << vmRow;

        if( vmRow.isEmpty() ){
            continue;
        }

        QStringList infoPair = vmRow.split(": ");

        QString key = infoPair.takeFirst().toLower();

        if( key.isEmpty() ){
            continue;
        }

        if ( key.operator ==("name")) {

            if (infoPair.count() > 1)  {
                continue;
            }

            QHash<QByteArray, QString> infoHash;

            m_vmHashList.append(infoHash);

            m_vmHashList.last().insert("name", infoPair.last() );

            continue;
        }


        if ( key.operator==("uuid") ) {
            m_vmHashList.last().insert("UUID", infoPair.last() );

            m_vmsUuidList.append(infoPair.last().toLatin1());

            continue;
        }

        if ( key.operator==("guest os") ){
            m_vmHashList.last().insert("ostype", infoPair.last() );
            continue;
        }

        if ( key.operator==("state") ){
            QString state = infoPair.last();
            state = state.left(state.indexOf("(") );
            m_vmHashList.last().insert("state", state );
            continue;
        }

        if ( key.operator==("memory size") ){
            m_vmHashList.last().insert("memory", infoPair.last() );
            continue;
        }

        if ( key.operator==("cpu exec cap") ){
            m_vmHashList.last().insert("cpumax", infoPair.last() );
            continue;
        }

    }

    return m_vmsUuidList;
}


///
// @return uuid, name, ostype, state, memory, cpumax
QHash<QByteArray, QString> VirtualBoxSshPlugin::listVmInfo( QByteArray id ) {

    return m_vmHashList.at(m_vmsUuidList.indexOf(id));

}





// Harddisc TODO





bool VirtualBoxSshPlugin::startVm( const QByteArray id ) const{
    QByteArray returnValue = executeInSSHShell("startvm " + id + " --type headless");
    qDebug() << returnValue;
    return true;
}

bool VirtualBoxSshPlugin::stopVm(const QByteArray id) const{
    QByteArray returnValue = executeInSSHShell("controlvm " + id + " poweroff");
    qDebug() << returnValue;
    return true;
}

bool VirtualBoxSshPlugin::copyVm( const QByteArray id, const QByteArray name ) const{
    QByteArray cloneVm = QByteArray("clonevm " + id + " --register --name " + name);

    QByteArray returnValue = executeInSSHShell(cloneVm);

    qDebug() << returnValue;

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

    qDebug() << "ssh Command: " << command;

    vBoxProcess.write(command);
    vBoxProcess.closeWriteChannel();

    vBoxProcess.waitForFinished();

    return vBoxProcess.readAll();
}



Q_EXPORT_PLUGIN2(virtualboxsshplugin, VirtualBoxSshPlugin)
