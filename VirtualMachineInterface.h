#ifndef VIRTUALMACHINEINTERFACE_H
#define VIRTUALMACHINEINTERFACE_H

#include <QtPlugin>
#include <QHash>
#include <QStringList>


class VirtualMachineInterface
{


public:

    virtual ~VirtualMachineInterface() {}

    virtual QString name() const = 0;

    virtual QString description() const = 0;

    virtual QList<QByteArray> listVmUUIDs() = 0;

    virtual QHash<QByteArray, QByteArray>  listVmInfo( QByteArray id ) = 0;

    virtual bool startVm( const QByteArray id ) const = 0;

    virtual bool copyVm( const QByteArray id, const QByteArray name ) const = 0;


    virtual void setLoginName(const QByteArray login) {
        m_login = login;
    }

    virtual void setHostName(const QByteArray host){
        m_hostname = host;
    }

//    virtual bool createVm( QStringList param ) const;



//    virtual bool deleteVm( int machine ) const;


//    virtual QHash<QByteArray> monitor( int machine ) const;



protected:


      QByteArray m_hostname;
      QByteArray m_login;

};


Q_DECLARE_INTERFACE(VirtualMachineInterface,
                    "com.erebetez.VirtualMachineManager.VirtualMachineInterface/1.0")

#endif // VIRTUALMACHINEINTERFACE_H
