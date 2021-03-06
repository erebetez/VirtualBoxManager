#-------------------------------------------------
#
# Project created by QtCreator 2012-02-01T21:35:44
#
#-------------------------------------------------

QT       += core gui sql

TARGET = VirtualMachineManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    vmstarter.cpp \
    settingsdialog.cpp \
    listdialog.cpp \
    sqlfilterlineedit.cpp \
    listview.cpp \
    listcustomsearch.cpp \
    sqltools.cpp \
    hypervisor.cpp \
    settings.cpp \
    databasehandler.cpp

HEADERS  += mainwindow.h \
    vmstarter.h \
    VirtualMachineInterface.h \
    settingsdialog.h \
    sqlfilterlineedit.h \
    listview.h \
    listdialog.h \
    listcustomsearch.h \
    sqltools.h \
    hypervisor.h \
    settings.h \
    databasehandler.h

FORMS    += mainwindow.ui \
    settingsdialog.ui

LIBS  = -L../plugins -lvirtualboxsshplugin -lhypervisordummyplugin

OTHER_FILES += \
    README.txt

OBJECTS_DIR = ../../build/.obj
MOC_DIR = ../../build/.moc
RCC_DIR = ../../build/.rcc
UI_DIR = ../../build/.ui

CONFIG(debug, debug|release) {
    DESTDIR = ../../debug
} else {
    DESTDIR = ../../release
}
