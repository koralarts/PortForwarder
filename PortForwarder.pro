#-------------------------------------------------
#
# Project created by QtCreator 2012-03-11T21:18:49
#
#-------------------------------------------------

QT       += core gui

TARGET = PortForwarder
TEMPLATE = app


SOURCES += main.cpp \
        mainwindow.cpp \
    socket.cpp \
    forwarding.cpp \
    epoll.cpp \
    client.cpp

HEADERS  += mainwindow.h \
    socket.h \
    forwarding.h \
    epoll.h \
    client.h

FORMS    += mainwindow.ui














