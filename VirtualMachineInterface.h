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

    virtual ~VirtualMachineInterface(){}

    virtual QString name() const { return QString(); }

    virtual QString description() const { return QString();}


    virtual QList<QByteArray> listVmUUIDs() const { return QList<QByteArray> (); }

    virtual QHash<QByteArray, QByteArray>  VlistVmInfo( QByteArray id ) { return QHash<QByteArray, QByteArray>  (); }

    virtual bool startVm( int machine ) const { return false; }


//    virtual bool createVm( QStringList param ) const;

//    virtual bool copyVm( int machine ) const;

//    virtual bool deleteVm( int machine ) const;


//    virtual QHash<QByteArray> monitor( int machine ) const;



protected:
      VirtualMachineInterface(){}



//    QString m_name;
//    QString m_description;

};


#endif // VIRTUALMACHINEINTERFACE_H
