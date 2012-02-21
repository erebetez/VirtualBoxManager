#include "hypervisordummy.h"

#include <QDebug>
//#include <QMessageBox>


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
    return QList<QByteArray>() << "20110121machine1" << "20110121machine3" << "20110121machine2";
}


///
// @return uuid, name, ostype, state, memory, cpumax
QHash<QByteArray, QString> HypervisorDummy::listVmInfo( QByteArray id ) {

    QHash<QByteArray, QString> infoHash;

    infoHash["UUID"] = "UUID " + id ;

    infoHash["name"] = "name " + id ;

    infoHash["ostype"] = "ostype " + id ;

    infoHash["state"] = "state " + id ;

    infoHash["memory"] = "memory " + id ;

    infoHash["cpumax"] = "cpumax " + id ;

    return infoHash;
}


// Harddisc TODO





bool HypervisorDummy::startVm( const QByteArray id ) const{
    qDebug() << "Dummy starting " + id;
    return true;
}

bool HypervisorDummy::copyVm(const QByteArray id, const QByteArray name) const {
    qDebug() << "Dummy copy " << id << "to" << name;
    return true;
}

Q_EXPORT_PLUGIN2(hypervisordummyplugin, HypervisorDummy)
