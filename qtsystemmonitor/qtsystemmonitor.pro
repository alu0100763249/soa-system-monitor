#-------------------------------------------------
#
# Project created by QtCreator 2017-02-21T12:13:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtsystemmonitor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mihilo.cpp \
    hardwareh.cpp

HEADERS  += mainwindow.h \
    mihilo.h \
    hardwareh.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=gnu++11
