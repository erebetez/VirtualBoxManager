#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include "settingsdialog.h"

class Settings : public QSettings
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);
    ~Settings();

    QString databasePath();

    QList<Hypervisor*>  hypervisors();

signals:
    void settingsChanged();

public slots:
    void showDialog();
    void save();

private:
    void saveHypervisors();

    SettingsDialog *m_settingsDialog;

};

#endif // SETTINGS_H
