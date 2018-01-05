#-------------------------------------------------
#
# Project created by QtCreator 2015-02-02T16:58:53
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Xavier
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingsDialog.cpp

HEADERS  += mainwindow.h \
    settingsDialog.h

RESOURCES += \
    myfiles.qrc

DISTFILES +=
