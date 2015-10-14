#-------------------------------------------------
#
# Project created by QtCreator 2015-10-02T13:01:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HuffmanCode
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    calculator.cpp \
    vertex.cpp \
    graph.cpp

HEADERS  += mainwindow.h \
    calculator.h \
    vertex.h \
    graph.h

FORMS    += mainwindow.ui

CONFIG += c++11
CONFIG += link_pkgconfig
PKGCONFIG += x11
