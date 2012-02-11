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
    virtualboxsshimpl.cpp \
    vmlistview/listdialog.cpp \
    vmlistview/sqlfilterlineedit.cpp \
    vmlistview/listview.cpp \
    vmlistview/listcustomsearch.cpp \
    vmlistview/sqltools.cpp

HEADERS  += mainwindow.h \
    vmstarter.h \
    VirtualMachineInterface.h \
    settingsdialog.h \
    virtualboxsshimpl.h \
    vmlistview/sqlfilterlineedit.h \
    vmlistview/listview.h \
    vmlistview/listdialog.h \
    vmlistview/listcustomsearch.h \
    vmlistview/sqltools.h

FORMS    += mainwindow.ui \
    settingsdialog.ui

OTHER_FILES += \
    README.txt
