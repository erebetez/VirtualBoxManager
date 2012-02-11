#ifndef VIRTUALBOXIMPL_H
#define VIRTUALBOXIMPL_H

#include "VirtualMachineInterface.h"



class VirtualBoxSSHImpl : public VirtualMachineInterface
{
public:

    static VirtualBoxSSHImpl *instance();
    virtual ~VirtualBoxSSHImpl(){}

    virtual QString name() const;

    virtual QString description() const;


    virtual QList<QByteArray>  listVmUUIDs();

    virtual QHash<QByteArray, QByteArray>   listVmInfo( QByteArray id );

    virtual bool startVm( int machine ) const;


private:
    VirtualBoxSSHImpl();


    QList<QByteArray> vBoxManageProcess( QStringList param ) const;

    QByteArray removeSurroundingChar( QByteArray string, const char removeChar );

};

#endif // VIRTUALBOXIMPL_H
