QT += testlib
QT += gui
QT += quick
CONFIG += qt warn_on depend_includepath testcase
CONFIG += console

TEMPLATE = app
TARGET = tst_dronetests
INCLUDEPATH += .
SOURCES +=  tst_dronetests.cpp ../../src/GestionDrones/Drone.cpp
DEFINES += SRCDIR=\\\"$$PWD\\\"
HEADERS += ../../src/GestionDrones/Drone.h
