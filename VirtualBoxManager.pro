#-------------------------------------------------
#
# Project created by QtCreator 2012-02-01T21:35:44
#
#-------------------------------------------------

QT       += core gui

TARGET = VirtualBoxManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    vmstarter.cpp \
    virtualboximpl.cpp

HEADERS  += mainwindow.h \
    vmstarter.h \
    VirtualMachineInterface.h \
    virtualboximpl.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    README.txt
