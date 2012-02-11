#ifndef VMSTARTER_H
#define VMSTARTER_H

#include <QWidget>

#include "virtualboxsshimpl.h"

class VmStarter: public QWidget
{
    Q_OBJECT

public:
    VmStarter(QWidget *parent);
    virtual ~VmStarter();

public slots:
    void startVirtualMachine();


private:
    VirtualBoxSSHImpl *m_vmInstance;

};

#endif // VMSTARTER_H
