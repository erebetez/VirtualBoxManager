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


    virtual QStringList listVm();

    virtual bool startVm( int machine ) const;


private:
    VirtualBoxImpl();

    QString vBoxProcess( const QStringList param ) const;

};

#endif // VIRTUALBOXIMPL_H
