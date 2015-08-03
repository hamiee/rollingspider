#-------------------------------------------------
#
# Project created by QtCreator 2015-05-01T16:40:30
#
#-------------------------------------------------

QT       += core gui opengl network printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

//INCLUDEPATH += /usr/local/qwt-6.1.2/include
//LIBS +=-L/usr/local/qwt-6.1.2/lib -lqwt

TARGET = untitled
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myglwidget.cpp \
    Tthread.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    myglwidget.h \
    Tthread.h \
    qcustomplot.h

FORMS    += mainwindow.ui
