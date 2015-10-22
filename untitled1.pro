#-------------------------------------------------
#
# Project created by QtCreator 2015-02-19T18:43:43
#
#-------------------------------------------------

QT       += core gui serialport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RC_FILE = myicon.rc

DISTFILES += \
    myicon.rc

RESOURCES += \
    res.qrc
