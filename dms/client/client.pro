QT +=core
QT -=gui

QMAKE_CXXFLAGS += -std=c++11

TARGET = client
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp	\
			client.cpp	\
			logReader.cpp	\
			logSender.cpp	\
    data.cpp \

HEADERS += client.h	\
			clientException.h	\
			data.h	\
			logReader.h	\
			logSender.h

