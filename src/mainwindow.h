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
    void startVm();
    void stopVm();
    void populateDataBase();

private slots:
    void loadPlugins();
    void reloadSettings();

private:
    Ui::MainWindow *ui;

    void setUpUI();
    void loadSettings();

    VmStarter *m_starter;

    Settings *m_settings;

//    QDir pluginsDir;
//    QStringList m_pluginFileNames;

    ListDialog *m_dockList;



};

#endif // MAINWINDOW_H
