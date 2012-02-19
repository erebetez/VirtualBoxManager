#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>

#include "settings.h"
#include "vmstarter.h"
#include "listdialog.h"
#include "hypervisor.h"

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
    void copyVm();    
    void loadPlugins();
    void populateDataBase();

private:
    Ui::MainWindow *ui;

    void setUpUI();
    void loadSettings();

    VmStarter *m_starter;

    Settings *m_settings;

//    QDir pluginsDir;
//    QStringList m_pluginFileNames;

    ListDialog *m_dockList;

//    QList<Hypervisor*> m_hypervisorList;

};

#endif // MAINWINDOW_H
