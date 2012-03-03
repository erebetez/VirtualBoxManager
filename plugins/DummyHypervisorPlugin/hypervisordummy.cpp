#include "hypervisordummy.h"

#include <QDebug>
#include <QUuid>

QByteArray HypervisorDummy::name() const {
    return QByteArray("Dummy");
}


QString HypervisorDummy::description() const {
    return QString("Dummy Hypervisor Implementation");
}

QString HypervisorDummy::info() const{
    return tr("A Dummy Hypervisor Implementation with some dummy vms");
}


QList<QByteArray>  HypervisorDummy::listVmUUIDs() {

    if( m_vmList.isEmpty() ){
        for(int i = 0; i < 4; ++i){
            m_vmList.append(QUuid::createUuid().toByteArray());
        }
    }
    return m_vmList;
}


///
// @return uuid, name, ostype, state, memory, cpumax
QHash<QByteArray, QString> HypervisorDummy::listVmInfo( QByteArray id ) {

    QHash<QByteArray, QString> infoHash;

    infoHash["UUID"] = id ;

    infoHash["name"] = "name" + id.right(3) ;

    infoHash["ostype"] = "haiku" ;

    infoHash["state"] = "running";

    infoHash["memory"] = "200";

    infoHash["cpumax"] = "100" ;

    return infoHash;
}


// Harddisc TODO





bool HypervisorDummy::startVm( const QByteArray id ) const{
    qDebug() << "Dummy starting " + id;
    return true;
}

bool HypervisorDummy::stopVm(const QByteArray id) const{
    qDebug() << "Dummy stopping " + id;
    return true;
}

bool HypervisorDummy::copyVm(const QByteArray id, const QByteArray name) const {
    qDebug() << "Dummy copy " << id << "to" << name;
    return true;
}

Q_EXPORT_PLUGIN2(hypervisordummyplugin, HypervisorDummy)
