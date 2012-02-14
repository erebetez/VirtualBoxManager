
 QT       -= gui

 TEMPLATE      = lib
 CONFIG       += plugin static
 INCLUDEPATH  += ../..
 HEADERS       = virtualboxsshplugin.h
 SOURCES       = virtualboxsshplugin.cpp
 TARGET        = $$qtLibraryTarget(virtualboxsshplugin)


OBJECTS_DIR = ../../../build/.obj
MOC_DIR = ../../../build/.moc
RCC_DIR = ../../../build/.rcc
UI_DIR = ../../../build/.ui

CONFIG(debug, debug|release) {
    DESTDIR = ..
} else {
    DESTDIR = ..
}
