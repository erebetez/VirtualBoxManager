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

    bool connectToDatabase(const QString &dataBaseFileName);

    QByteArray getHypervisorForMachineId(const QByteArray id);

Q_SIGNALS:
    void dbRefreshed();

public slots:
    void populateDb(QList<Hypervisor*> hypervisorList);


private:
    void clearDatabase() const;
    void initDatabase() const;

    VirtualMachineInterface* vmInstance(QByteArray typ) const;

};

#endif // VMSTARTER_H
