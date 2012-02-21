
 QT       -= gui

 TEMPLATE      = lib
 CONFIG       += plugin static
 INCLUDEPATH  += ../..
 HEADERS       = hypervisordummy.h
 SOURCES       = hypervisordummy.cpp
 TARGET        = $$qtLibraryTarget(hypervisordummyplugin)


OBJECTS_DIR = ../../../build/.obj
MOC_DIR = ../../../build/.moc
RCC_DIR = ../../../build/.rcc
UI_DIR = ../../../build/.ui

CONFIG(debug, debug|release) {
    DESTDIR = ..
} else {
    DESTDIR = ..
}
