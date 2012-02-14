#-------------------------------------------------
#
# Project created by QtCreator 2012-02-01T21:35:44
#
#-------------------------------------------------

QT       += core gui sql

TARGET = VirtualBoxManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    vmstarter.cpp \
    settingsdialog.cpp \
    listdialog.cpp \
    sqlfilterlineedit.cpp \
    listview.cpp \
    listcustomsearch.cpp \
    sqltools.cpp

HEADERS  += mainwindow.h \
    vmstarter.h \
    VirtualMachineInterface.h \
    settingsdialog.h \
    sqlfilterlineedit.h \
    listview.h \
    listdialog.h \
    listcustomsearch.h \
    sqltools.h

FORMS    += mainwindow.ui \
    settingsdialog.ui

LIBS  = -L../VirtualBoxManager/plugins -lvirtualboxsshplugin


OTHER_FILES += \
    README.txt
