#-------------------------------------------------
#
# Project created by QtCreator 2014-02-12T10:41:30
#
#-------------------------------------------------

QT       += core gui webkit xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = qtMediaElch2HTML
TEMPLATE = app


SOURCES += main.cpp\
        cmainwindow.cpp \
    csourcescombodelegate.cpp \
    csourcesbuttondelegate.cpp \
    cpushbutton.cpp \
    cmovie.cpp \
    common.cpp \
    ctvshow.cpp \
    ctvshowepisode.cpp \
    cxbmc.cpp \
    cgenrecombine.cpp

HEADERS  += cmainwindow.h \
    csourcescombodelegate.h \
    csourcesbuttondelegate.h \
    cpushbutton.h \
    cmovie.h \
    common.h \
    ctvshow.h \
    ctvshowepisode.h \
    cxbmc.h \
    cgenrecombine.h

FORMS    += cmainwindow.ui

RESOURCES += \
    template.qrc
