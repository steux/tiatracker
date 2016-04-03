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
    track/instrument.cpp \
    track/track.cpp \
    instrumentstab.cpp \
    emulation/player.cpp \
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
    track/sequence.cpp \
    track/sequenceentry.cpp \
    patterneditor.cpp \
    timeline.cpp \
    setslidedialog.cpp \
    setfrequencydialog.cpp \
    renamepatterndialog.cpp \
    setgotodialog.cpp \
    insertpatterndialog.cpp \
    createpatterndialog.cpp \
    optionstab.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    pianokeyboard.h \
    tiasound/tiasound.h \
    tiasound/instrumentpitchguide.h \
    tiasound/pitchguide.h \
    tiasound/pitchguidefactory.h \
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
    track/sequence.h \
    track/sequenceentry.h \
    patterneditor.h \
    timeline.h \
    setslidedialog.h \
    setfrequencydialog.h \
    renamepatterndialog.h \
    setgotodialog.h \
    insertpatterndialog.h \
    createpatterndialog.h \
    optionstab.h \
    aboutdialog.h


FORMS    += mainwindow.ui \
    setslidedialog.ui \
    setfrequencydialog.ui \
    renamepatterndialog.ui \
    setgotodialog.ui \
    insertpatterndialog.ui \
    createpatterndialog.ui \
    aboutdialog.ui

CONFIG += c++11

RESOURCES += \
    main.qrc

DISTFILES += \
    style.qss \
    notes.txt \
    data/keymap.cfg \
    data/license.txt

win32: LIBS += -L$$PWD/sdl/windows/lib/ -lSDL2
linux: LIBS += -lSDL2

INCLUDEPATH += $$PWD/sdl/windows/include
DEPENDPATH += $$PWD/sdl/windows/include

# Copy files to output directory
install_it.path = $$OUT_PWD
install_it.files = $$PWD/data/*

INSTALLS += \
    install_it

# Copy player to output directory
install_player.path = $$OUT_PWD
install_player.files = $$PWD/player

INSTALLS += \
    install_player

RC_ICONS = graphics/tt_icon.ico
