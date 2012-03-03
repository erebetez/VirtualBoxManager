#ifndef VMSTARTER_H
#define VMSTARTER_H

#include "hypervisor.h"
#include "VirtualMachineInterface.h"

#include <QObject>



class VmStarter: public QObject
{
    Q_OBJECT

public:
    VmStarter(QObject *parent);
    virtual ~VmStarter();

    void setHypervisorList(const QList<Hypervisor*> list);

Q_SIGNALS:
    void dbRefreshed();

public slots:
    void populateVmsDb();
    void start(const QByteArray id);
    void stop(const QByteArray id);
    void copy(const QByteArray id);

private:
    VirtualMachineInterface* vmInstance(Hypervisor *hy) const;

    QByteArray getHypervisorForMachineId(const QByteArray id) const;

    QList<Hypervisor*> m_hypervisorList;

};

#endif // VMSTARTER_H
