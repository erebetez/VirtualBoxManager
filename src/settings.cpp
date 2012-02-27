#include "settings.h"

#include "hypervisor.h"

#include <QDebug>

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

    remove("Hypervisors");

    beginGroup("Hypervisors");

    foreach(Hypervisor *hy, m_settingsDialog->hypervisors()){
        beginGroup(hy->name());

        setValue( "typ", hy->typ() );
        setValue( "adress", hy->adress());
        setValue( "user", hy->user());

        endGroup();
    }

    endGroup();
}



QList<Hypervisor*> Settings::hypervisors(){

    QList<Hypervisor*> hypervisorList;

    QString currentName;
    QString property;

    beginGroup("Hypervisors");

    foreach(QString key, allKeys()){

        if( key.split("/").first() != currentName ){
           currentName = key.split("/").first();

           Hypervisor *hy = new Hypervisor();
           hypervisorList.append(hy);

           hy->setName(currentName.toLatin1());

        }

        property = key.split("/").last();

        if(property == "typ"){
            hypervisorList.last()->setTyp(value(key, "").toString().toLatin1());
        }
        if(property == "adress"){
            hypervisorList.last()->setAdress(value(key, "").toString().toLatin1());
        }
        if(property == "user"){
            hypervisorList.last()->setUser(value(key, "").toString().toLatin1());
        }

    }

    endGroup();

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

