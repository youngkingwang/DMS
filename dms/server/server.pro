#-------------------------------------------------
#
# Project created by QtCreator 2014-02-05T21:59:54
#
#-------------------------------------------------

QT       += core

QT       -= gui

QT	+= sql

TARGET = server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11


SOURCES += main.cpp \
    datarecivethread.cpp \
    datasavethread.cpp \
    logdao.cpp \
    userdata.cpp \
    server.cpp

HEADERS += \
    datarecivethread.h \
    datasavethread.h \
    logdao.h \
    userdata.h \
    server.h
