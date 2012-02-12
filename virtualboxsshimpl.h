#ifndef VIRTUALBOXIMPL_H
#define VIRTUALBOXIMPL_H

#include "VirtualMachineInterface.h"





class VirtualBoxSSHImpl : public VirtualMachineInterface
{
public:

    static VirtualBoxSSHImpl *instance();

    virtual ~VirtualBoxSSHImpl();

    virtual QString name() const;

    virtual QString description() const;

    virtual QList<QByteArray>  listVmUUIDs();

    virtual QHash<QByteArray, QByteArray> listVmInfo( QByteArray id );

    virtual bool startVm( const QByteArray id ) const;

    virtual bool copyVm( const QByteArray id, const QByteArray name ) const;

private:
    VirtualBoxSSHImpl();


    QList<QByteArray> vBoxManageProcess( QByteArray command ) const;

    QByteArray executeInSSHShell( QByteArray param ) const;

    QByteArray removeSurroundingChar( QByteArray string, const char removeChar );

};

#endif // VIRTUALBOXIMPL_H
