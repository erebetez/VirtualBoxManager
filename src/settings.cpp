#include "settings.h"

#include "hypervisor.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

Settings::Settings(QObject *parent) :
    QSettings(QSettings::IniFormat,  QSettings::UserScope, "VMachineManager", "VMachineManagerSettings", parent)
{

    qDebug() << fileName();

    m_settingsDialog = 0;

}

Settings::~Settings(){
    if( m_settingsDialog != 0){
        delete m_settingsDialog;
    }
}

QString Settings::databasePath(){
    // Needs to write a value and sync it in order to create the folder the first time the program is started.
    // Database will not open otherwiese.
    setValue("title", "hi there");
    sync();

    QString settingsPath = fileName();
    settingsPath = settingsPath.left(settingsPath.length() - QString("VMachineManagerSettings.ini").length() );

    return settingsPath + "VMachineManagerSettings.db";
}

void Settings::save(){
    saveHypervisors();
    sync();
}


void Settings::saveHypervisors(){

    QSqlQuery query;
    query.exec("DELETE FROM hypervisors");
    query.prepare("INSERT INTO hypervisors (hypervisor, host, typ, user) "
                  "VALUES (?, ?, ?, ?)");

    foreach(Hypervisor *hy, m_settingsDialog->hypervisors()){
        query.addBindValue( hy->name() );
        query.addBindValue( hy->adress() );
        query.addBindValue( hy->typ() );
        query.addBindValue( hy->user() );
        query.exec();
    }

}



QList<Hypervisor*> Settings::hypervisors(){

    QList<Hypervisor*> hypervisorList;

    QSqlQuery query("SELECT hypervisor, host, typ, user FROM hypervisors");

    while ( query.next() ) {

        Hypervisor *hy = new Hypervisor();
        hypervisorList.append(hy);

        hypervisorList.last()->setName(query.value(0).toString().toLatin1());
        hypervisorList.last()->setAdress(query.value(1).toString().toLatin1());
        hypervisorList.last()->setTyp(query.value(2).toString().toLatin1());
        hypervisorList.last()->setUser(query.value(3).toString().toLatin1());
    }

    return hypervisorList;
}


void  Settings::showDialog(){

    if( m_settingsDialog == 0 ){
        m_settingsDialog = new SettingsDialog();
        m_settingsDialog->setModal(true);
        connect(m_settingsDialog, SIGNAL(accepted()), this, SLOT(save()));
    }
    m_settingsDialog->setHypervisors(hypervisors());
    m_settingsDialog->show();

}

