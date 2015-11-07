#-------------------------------------------------
#
# Project created by QtCreator 2015-10-24T08:35:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TIATracker
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pianokeyboard.cpp \
    tiasound/tiasound.cpp \
    tiasound/instrumentpitchguide.cpp \
    tiasound/pitchguide.cpp \
    tiasound/pitchguidefactory.cpp \
    tiasound/pitchperfectpal.cpp

HEADERS  += mainwindow.h \
    pianokeyboard.h \
    tiasound/tiasound.h \
    tiasound/instrumentpitchguide.h \
    tiasound/pitchguide.h \
    tiasound/pitchguidefactory.h \
    tiasound/pitchperfectpal.h

FORMS    += mainwindow.ui

CONFIG += c++11
