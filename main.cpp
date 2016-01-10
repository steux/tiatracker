#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QString>
#include <QComboBox>
#include <QThread>

#include "tiasound/tiasound.h"
#include "pianokeyboard.h"
#include "tiasound/pitchperfectpal.h"
#include "envelopeshaper.h"
#include "track/track.h"
#include "instrumentstab.h"
#include "percussiontab.h"
#include "tracktab.h"
#include "emulation/player.h"
#include <QThread>

#include <iostream>

#include "SDL.h"
#undef main
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Load and set stylesheet
    QFile styleFile(":/style.qss");
    styleFile.open(QFile::ReadOnly);
    a.setStyleSheet(styleFile.readAll());

    // Track
    Track::Track myTrack{};

    // GUI
    MainWindow w;
    w.initConnections();

    w.registerTrack(&myTrack);

    InstrumentsTab *it = w.findChild<InstrumentsTab *>("tabInstruments");
    it->registerTrack(&myTrack);
    it->initInstrumentsTab();
    it->updateInstrumentsTab();

    PercussionTab *pt = w.findChild<PercussionTab *>("tabPercussion");
    pt->registerTrack(&myTrack);
    pt->initPercussionTab();
    pt->updatePercussionTab();

    TrackTab *tt = w.findChild<TrackTab *>("tabTrack");
    tt->registerTrack(&myTrack);
    tt->initTrackTab();
//    tt->updateTrackTab();

    /* Create and initialize player thread */
    Emulation::Player tiaPlayer(&myTrack);
    QObject::connect(&w, SIGNAL(playInstrument(Track::Instrument*,int)), &tiaPlayer, SLOT(playInstrument(Track::Instrument*,int)));
    QObject::connect(&w, SIGNAL(stopInstrument()), &tiaPlayer, SLOT(stopInstrument()));
    QObject::connect(pt, SIGNAL(playWaveform(TiaSound::Distortion,int,int)), &tiaPlayer, SLOT(playWaveform(TiaSound::Distortion,int,int)));
    QObject::connect(&w, SIGNAL(playPercussion(Track::Percussion*)), &tiaPlayer, SLOT(playPercussion(Track::Percussion*)));
    QObject::connect(&w, SIGNAL(stopPercussion()), &tiaPlayer, SLOT(stopPercussion()));
    pt->connectPlayer(&tiaPlayer);
    tiaPlayer.run();

    // Shrink window size to minimum and show
    w.resize(0, 0);
    w.show();

    return a.exec();
}
