#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "VirtualMachineInterface.h"


#include <QPluginLoader>
#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    qDebug() << "Settings konstruktor.";

    ui->setupUi(this);


    ui->labelInfo->setText(QString());


    foreach (QObject *plugin, QPluginLoader::staticInstances()){
        VirtualMachineInterface *iVBox = qobject_cast<VirtualMachineInterface *>(plugin);
        ui->comboBoxVmEngine->addItem(iVBox->name()); //description()
     }

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(saveSettings()));
    connect(ui->pushButtonSave, SIGNAL(pressed()), this, SLOT(saveHypervisor()));
    connect(ui->listHypervisorHosts, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)) ,
            this, SLOT(selectedHypervisor(QListWidgetItem*,QListWidgetItem*)));
}


void SettingsDialog::saveSettings(){


}

void SettingsDialog::setHypervisors( QList<Hypervisor*> &hypervisorList ){
    m_hypervisorList = hypervisorList;
}

void SettingsDialog::populateHypervisorList(){
    ui->listHypervisorHosts->clear();

    foreach(Hypervisor *hi, m_hypervisorList){
        ui->listHypervisorHosts->addItem( hi->name() );
    }
}

void SettingsDialog::saveHypervisor(){

    foreach(Hypervisor *hi, m_hypervisorList){
        if(hi->name() == ui->lineEditName){
           hi->setAdress(ui->lineEditHost->text().toLatin1());
           hi->setTyp(ui->comboBoxVmEngine->currentText().toLatin1());
           hi->setUser(ui->lineEditUser->text().toLatin1());
           return;
        }
    }

    // new one
    Hypervisor *newHi = new Hypervisor();
    newHi->setName(ui->lineEditName->text().toLatin1());
    newHi->setAdress(ui->lineEditHost->text().toLatin1());
    newHi->setTyp(ui->comboBoxVmEngine->currentText().toLatin1());
    newHi->setUser(ui->lineEditUser->text().toLatin1());
    m_hypervisorList.append(newHi);

    populateHypervisorList();
}

void SettingsDialog::selectedHypervisor(QListWidgetItem* current, QListWidgetItem* previous){

    Q_UNUSED(previous)

    foreach(Hypervisor *hi, m_hypervisorList){
        if(hi->name() == current->text()){
           ui->lineEditHost->setText(hi->adress());
           ui->lineEditName->setText(hi->name());
           ui->lineEditUser->setText(hi->user());
           ui->comboBoxVmEngine->addItem(hi->typ());
        }
    }

}


SettingsDialog::~SettingsDialog()
{
    delete ui;
}
