#ifndef VIRTUALBOXIMPL_H
#define VIRTUALBOXIMPL_H

#include "VirtualMachineInterface.h"



class VirtualBoxImpl : public VirtualMachineInterface
{
public:

    static VirtualBoxImpl *instance();
    virtual ~VirtualBoxImpl(){}

    virtual QString name() const;

    virtual QString description() const;


    virtual QList<QByteArray>  listVmUUIDs();

    virtual QHash<QByteArray, QByteArray>   listVmInfo( QByteArray id );

    virtual bool startVm( int machine ) const;


private:
    VirtualBoxImpl();


    QList<QByteArray> vBoxManageProcess( const QStringList param ) const;

    QByteArray removeSurroundingChar( QByteArray string, const char removeChar );

};

#endif // VIRTUALBOXIMPL_H
