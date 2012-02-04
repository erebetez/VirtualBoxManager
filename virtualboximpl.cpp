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


QStringList VirtualBoxImpl::listVm() {

    QString returnValue = vBoxProcess( QStringList() << "list" << "vms" );

    return returnValue.split('\n',QString::SkipEmptyParts);

}

bool VirtualBoxImpl::startVm(int machine) const{
    return false;
}


QString VirtualBoxImpl::vBoxProcess( const QStringList param ) const {
    QProcess vBoxProcess;

    vBoxProcess.start("VBoxManage", param );

    vBoxProcess.waitForFinished();

    return QString(vBoxProcess.readAll());

}
