#-------------------------------------------------
#
# Project created by QtCreator 2016-09-26T14:07:36
#
#-------------------------------------------------
QT       += core gui serialport
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = game_ger
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serial_port.cpp \
    appengine.cpp \
    collect_data.cpp \
    sdl_window.cpp

#LIBS += -L/usr/local/lib -lSDL2
# INCLUDES += /usr/local/include

LIBS += -L/usr/local/lib -lSDL2 -ldl -lpthread
INCLUDES += /usr/include/SDL

HEADERS  += mainwindow.h \
    serial_port.h \
    appengine.h \
    collect_data.h \
    sdl_window.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

