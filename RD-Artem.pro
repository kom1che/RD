#-------------------------------------------------
#
# Project created by QtCreator 2016-08-29T13:45:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RD-Artem
TEMPLATE = app


SOURCES += main.cpp\
        service.cpp \
    originfpl.cpp \
    anime.cpp

HEADERS  += service.h \
    originfpl.h \
    anime.h

FORMS    += service.ui

RESOURCES += \
    src.qrc
