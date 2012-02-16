#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include <QSettings>

#include "hypervisor.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();
    
    void setHypervisors(QList<Hypervisor*> &hypervisorList);

private slots:
    void saveSettings();
    void saveHypervisor();
    void selectedHypervisor(QListWidgetItem*,QListWidgetItem*);

private:
    Ui::SettingsDialog *ui;

    void populateHypervisorList();

    QList<Hypervisor*> m_hypervisorList;

};

#endif // SETTINGSDIALOG_H
