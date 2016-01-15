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
    tiasound/pitchperfectpal.cpp \
    track/instrument.cpp \
    track/track.cpp \
    instrumentstab.cpp \
    emulation/Player.cpp \
    emulation/SoundSDL2.cpp \
    emulation/TIASnd.cpp \
    envelopeshaper.cpp \
    percussiontab.cpp \
    track/percussion.cpp \
    percussionshaper.cpp \
    waveformshaper.cpp \
    tracktab.cpp \
    instrumentselector.cpp \
    track/note.cpp \
    track/pattern.cpp \
    track/sequence.cpp

HEADERS  += mainwindow.h \
    pianokeyboard.h \
    tiasound/tiasound.h \
    tiasound/instrumentpitchguide.h \
    tiasound/pitchguide.h \
    tiasound/pitchguidefactory.h \
    tiasound/pitchperfectpal.h \
    track/instrument.h \
    track/track.h \
    instrumentstab.h \
    emulation/player.h \
    emulation/bspf.h \
    emulation/SoundSDL2.h \
    emulation/TIASnd.h \
    envelopeshaper.h \
    percussiontab.h \
    track/percussion.h \
    percussionshaper.h \
    waveformshaper.h \
    tracktab.h \
    instrumentselector.h \
    track/note.h \
    track/pattern.h \
    track/sequence.h

FORMS    += mainwindow.ui

CONFIG += c++11

RESOURCES += \
    main.qrc

DISTFILES += \
    style.qss

win32: LIBS += -L$$PWD/sdl/windows/lib/ -lSDL2

INCLUDEPATH += $$PWD/sdl/windows/include
DEPENDPATH += $$PWD/sdl/windows/include
