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

    connect(ui->pushButtonSave, SIGNAL(pressed()), this, SLOT(saveHypervisor()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(saveHypervisor()));
    connect(ui->pushButtonDelete, SIGNAL(pressed()), this, SLOT(deleteHypervisor()));

    connect(ui->listHypervisorHosts, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)) ,
            this, SLOT(selectedHypervisor(QListWidgetItem*,QListWidgetItem*)));
}


void SettingsDialog::setHypervisors( QList<Hypervisor*> hypervisorList ){
    m_hypervisorList = hypervisorList;
    populateHypervisorList();
}

QList<Hypervisor*> SettingsDialog::hypervisors() const{
    return m_hypervisorList;
}

void SettingsDialog::populateHypervisorList(){
    ui->listHypervisorHosts->clear();

    foreach(Hypervisor *hy, m_hypervisorList){
        ui->listHypervisorHosts->addItem( hy->name() );
    }
}

void SettingsDialog::saveHypervisor(){

    // Check if the name already exixts
    foreach(Hypervisor *hy, m_hypervisorList){
        if(hy->name() == ui->lineEditName->text().toLatin1()){
           hy->setAdress(ui->lineEditHost->text().toLatin1());
           hy->setTyp(ui->comboBoxVmEngine->currentText().toLatin1());
           hy->setUser(ui->lineEditUser->text().toLatin1());
           return;
        }
    }

    // new one
    Hypervisor *newHy = new Hypervisor();
    newHy->setName(ui->lineEditName->text().toLatin1());
    newHy->setAdress(ui->lineEditHost->text().toLatin1());
    newHy->setTyp(ui->comboBoxVmEngine->currentText().toLatin1());
    newHy->setUser(ui->lineEditUser->text().toLatin1());
    m_hypervisorList.append(newHy);

    populateHypervisorList();
}

void SettingsDialog::deleteHypervisor(){

    for(int i = 0; i < m_hypervisorList.count(); ++i){
        if( m_hypervisorList.at(i)->name() == ui->lineEditName->text().toLatin1() ){
            delete m_hypervisorList.at(i);
            m_hypervisorList.removeAt(i) ;

        }
    }
    populateHypervisorList();
}

void SettingsDialog::selectedHypervisor(QListWidgetItem* current, QListWidgetItem* previous){

    Q_UNUSED(previous)

    if( current == 0) {
        return;
    }

    foreach(Hypervisor *hy, m_hypervisorList){

        if(hy->name() == current->text().toLatin1()){
           ui->lineEditHost->setText(hy->adress());
           ui->lineEditName->setText(hy->name());
           ui->lineEditUser->setText(hy->user());
           for ( int i = 0; i < ui->comboBoxVmEngine->count(); ++i) {
              if( ui->comboBoxVmEngine->itemText(i).toLatin1() == hy->typ() ){
                  ui->comboBoxVmEngine->setCurrentIndex(i);
              }
           }

        }
    }
}


SettingsDialog::~SettingsDialog()
{
    delete ui;
}
