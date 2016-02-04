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
#include "track/note.h"
#include "track/pattern.h"
#include "track/sequence.h"
#include "track/sequenceentry.h"

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
    // Test track
    // Ins 0: lead, 1: base drum, 2: hi drum
    Track::Pattern patDrums("Drum loop");
    patDrums.notes = QList<Track::Note>{
        {Track::Note::instrumentType::Percussion, 0, 0},
        {Track::Note::instrumentType::Hold, 0, 0},
        {Track::Note::instrumentType::Percussion, 1, 0},
        {Track::Note::instrumentType::Hold, 0, 0},
        {Track::Note::instrumentType::Hold, 0, 0},
        {Track::Note::instrumentType::Hold, 0, 0},
        {Track::Note::instrumentType::Percussion, 1, 0},
        {Track::Note::instrumentType::Hold, 0, 0}
    };
    Track::Pattern patMelody1("Melody 1");
    patMelody1.notes = QList<Track::Note>{
        {Track::Note::instrumentType::Instrument, 0, 8},
        {Track::Note::instrumentType::Hold, 0, 0},
        {Track::Note::instrumentType::Hold, 0, 0},
        {Track::Note::instrumentType::Hold, 0, 0},
        {Track::Note::instrumentType::Instrument, 0, 16},
        {Track::Note::instrumentType::Hold, 0, 0},
        {Track::Note::instrumentType::Hold, 0, 0},
        {Track::Note::instrumentType::Hold, 0, 0},
        {Track::Note::instrumentType::Instrument, 0, 24},
        {Track::Note::instrumentType::Slide, 0, -1},
        {Track::Note::instrumentType::Slide, 0, -2},
        {Track::Note::instrumentType::Slide, 0, -3},
        {Track::Note::instrumentType::Instrument, 0, 31},
        {Track::Note::instrumentType::Slide, 0, 1},
        {Track::Note::instrumentType::Slide, 0, 2},
        {Track::Note::instrumentType::Slide, 0, 3}
    };
    Track::Pattern patMelody2("Melody 2");
    patMelody2.notes = QList<Track::Note>{
        {Track::Note::instrumentType::Instrument, 0, 8},
        {Track::Note::instrumentType::Hold, 0, 0},
        {Track::Note::instrumentType::Pause, 0, 0},
        {Track::Note::instrumentType::Pause, 0, 0},
        {Track::Note::instrumentType::Instrument, 0, 16},
        {Track::Note::instrumentType::Hold, 0, 0},
        {Track::Note::instrumentType::Pause, 0, 0},
        {Track::Note::instrumentType::Pause, 0, 0},
        {Track::Note::instrumentType::Instrument, 0, 24},
        {Track::Note::instrumentType::Hold, 0, 0},
        {Track::Note::instrumentType::Pause, 0, 0},
        {Track::Note::instrumentType::Pause, 0, 0},
        {Track::Note::instrumentType::Instrument, 0, 31},
        {Track::Note::instrumentType::Hold, 0, 0},
        {Track::Note::instrumentType::Pause, 0, 0},
        {Track::Note::instrumentType::Pause, 0, 0}
    };

    myTrack.patterns.append(patDrums);
    myTrack.patterns.append(patMelody1);
    myTrack.patterns.append(patMelody2);

    Track::SequenceEntry intro0{0};
    Track::SequenceEntry intro1{0};
    myTrack.channelSequences[0].sequence.append(intro0);
    myTrack.channelSequences[1].sequence.append(intro1);
    Track::SequenceEntry melody1_0{1};
    Track::SequenceEntry melody1_10{0};
    Track::SequenceEntry melody1_11{0};
    myTrack.channelSequences[0].sequence.append(melody1_0);
    myTrack.channelSequences[1].sequence.append(melody1_10);
    myTrack.channelSequences[1].sequence.append(melody1_11);
    Track::SequenceEntry melody2_0{2, 1};
    Track::SequenceEntry melody2_10{0};
    Track::SequenceEntry melody2_11{0, 1};
    myTrack.channelSequences[0].sequence.append(melody2_0);
    myTrack.channelSequences[1].sequence.append(melody2_10);
    myTrack.channelSequences[1].sequence.append(melody2_11);

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
    tt->registerPitchGuide(w.getPitchGuide());
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
