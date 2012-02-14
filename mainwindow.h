#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>

#include "settingsdialog.h"
#include "vmstarter.h"
#include "listdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void showSettings();
    void copyVm();    
    void loadPlugins();

private:
    Ui::MainWindow *ui;



    void setUpUI();

    VmStarter *m_starter;

    QDir pluginsDir;
    QStringList m_pluginFileNames;

    ListDialog *m_dockList;
    SettingsDialog *m_settings;

};

#endif // MAINWINDOW_H
