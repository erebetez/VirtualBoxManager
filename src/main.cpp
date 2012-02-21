#include <QtGui/QApplication>
#include <QtPlugin>
#include "mainwindow.h"

Q_IMPORT_PLUGIN(virtualboxsshplugin)
Q_IMPORT_PLUGIN(hypervisordummyplugin)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
