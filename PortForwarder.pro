#-------------------------------------------------
#
# Project created by QtCreator 2012-03-11T21:18:49
#
#-------------------------------------------------

QT       += core gui

TARGET = PortForwarder
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    socket.cpp \
    forwarding.cpp \
    epoll.c

HEADERS  += mainwindow.h \
    socket.h \
    forwarding.h \
    epoll.h

FORMS    += mainwindow.ui










