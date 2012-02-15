#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "VirtualMachineInterface.h"

#include <QPluginLoader>
#include <QDebug>

SettingsDialog::SettingsDialog(QSettings *settingsObject, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    qDebug() << "Settings konstruktor.";

    ui->setupUi(this);
    m_settings = settingsObject;


    ui->labelInfo->setText(QString());


    foreach (QObject *plugin, QPluginLoader::staticInstances()){
        VirtualMachineInterface *iVBox = qobject_cast<VirtualMachineInterface *>(plugin);
        ui->comboBoxVmEngine->addItem(iVBox->description());
    }

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(saveSettings()));
    connect(ui->pushButtonSave, SIGNAL(pressed()), this, SLOT(saveHypervisor()));
}

void SettingsDialog::saveSettings(){



}

void SettingsDialog::saveHypervisor(){
    m_settings->beginGroup("Hypervisors");
    m_settings->setValue("","");


}


SettingsDialog::~SettingsDialog()
{
    delete ui;
}
