#ifndef VIRTUALBOXIMPL_H
#define VIRTUALBOXIMPL_H

#include <src/VirtualMachineInterface.h>

#include <QObject>

class VirtualBoxSshPlugin : public QObject, public VirtualMachineInterface
{
    Q_OBJECT
    Q_INTERFACES(VirtualMachineInterface)

public:
    QString name() const;

    QString description() const;

    QList<QByteArray>  listVmUUIDs();

    QHash<QByteArray, QByteArray> listVmInfo( QByteArray id );

    bool startVm( const QByteArray id ) const;

    bool copyVm( const QByteArray id, const QByteArray name ) const;

private:

    QList<QByteArray> vBoxManageProcess( QByteArray command ) const;

    QByteArray executeInSSHShell( QByteArray param ) const;

    QByteArray removeSurroundingChar( QByteArray string, const char removeChar );

};

#endif // VIRTUALBOXIMPL_H