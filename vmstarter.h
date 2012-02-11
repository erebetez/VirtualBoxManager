#ifndef VMSTARTER_H
#define VMSTARTER_H

#include <QObject>

#include "virtualboxsshimpl.h"

class VmStarter: public QObject
{
    Q_OBJECT

public:
    VmStarter(QObject *parent);
    virtual ~VmStarter();

Q_SIGNALS:
    void dbRefreshed();

public slots:
    void populateDb();

private:
    VirtualBoxSSHImpl *m_vmInstance;

    bool connectToDatabase();
    void initDatabase();

};

#endif // VMSTARTER_H
