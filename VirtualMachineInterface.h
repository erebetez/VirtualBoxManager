#ifndef VIRTUALMACHINEINTERFACE_H
#define VIRTUALMACHINEINTERFACE_H

#include <QHash>
#include <QStringList>
#include <QObject>


class VirtualMachineInterface : public QObject
{
Q_OBJECT

public:
    static VirtualMachineInterface *instance() { return 0; }

    virtual ~VirtualMachineInterface() {}

    virtual QString name() const = 0;

    virtual QString description() const = 0;

    virtual QList<QByteArray> listVmUUIDs() = 0;

    virtual QHash<QByteArray, QByteArray>  listVmInfo( QByteArray id ) = 0;

    virtual bool startVm( QByteArray id ) const = 0;

    virtual bool copyVm( QByteArray id ) const = 0;


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
      VirtualMachineInterface(){}

      QByteArray m_hostname;
      QByteArray m_login;

};


#endif // VIRTUALMACHINEINTERFACE_H
