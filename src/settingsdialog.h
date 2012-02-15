#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingsDialog(QSettings *settingsObject, QWidget *parent = 0);
    ~SettingsDialog();
    
private slots:
    void saveSettings();
    void saveHypervisor();

private:
    Ui::SettingsDialog *ui;
    QSettings *m_settings;
};

#endif // SETTINGSDIALOG_H
