#ifndef HYPERVISORDUMMY_H
#define HYPERVISORDUMMY_H

#include <src/VirtualMachineInterface.h>

#include <QObject>

class HypervisorDummy : public QObject, public VirtualMachineInterface
{
    Q_OBJECT
    Q_INTERFACES(VirtualMachineInterface)

public:
    QByteArray name() const;

    QString description() const;

    QString info() const;

    QList<QByteArray>  listVmUUIDs();

    QHash<QByteArray, QString> listVmInfo( QByteArray id );

    bool startVm( const QByteArray id ) const;

    bool copyVm( const QByteArray id, const QByteArray name ) const;

private:

};

#endif // HYPERVISORDUMMY_H
