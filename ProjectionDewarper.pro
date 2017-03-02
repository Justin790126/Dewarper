#-------------------------------------------------
#
# Project created by QtCreator 2017-02-23T13:52:22
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = ProjectionDewarper
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv \
                /usr/local/include/opencv2

LIBS += -L/usr/local/lib/

LIBS += -lopencv_highgui
LIBS += -lopencv_core
LIBS += -lopencv_imgproc
LIBS += -lopencv_imgcodecs


SOURCES += main.cpp \
    dewarper.cpp \
    mainwindow.cpp

HEADERS += \
    dewarper.h \
    mainwindow.h

FORMS += \
    mainwindow.ui
