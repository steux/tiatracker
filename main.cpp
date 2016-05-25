/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QString>
#include <QComboBox>
#include <QThread>

#include "tiasound/tiasound.h"
#include "pianokeyboard.h"
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
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include "timeline.h"
#include <iostream>
#include <patterneditor.h>
#include <QCheckBox>
#include "optionstab.h"


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
    myTrack.newTrack();

    // GUI
    MainWindow::loadKeymap();
    MainWindow w;
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
    tt->updateTrackTab();

    OptionsTab *ot = w.findChild<OptionsTab *>("tabOptions");
    ot->registerTrack(&myTrack);
    ot->initOptionsTab();
    ot->updateOptionsTab();

    w.initConnections();

    /* Create and initialize player thread */
    Emulation::Player tiaPlayer(&myTrack);
    QObject::connect(&w, SIGNAL(playInstrument(Track::Instrument*,int)), &tiaPlayer, SLOT(playInstrument(Track::Instrument*,int)));
    QObject::connect(&w, SIGNAL(playInstrumentOnce(Track::Instrument*,int)), &tiaPlayer, SLOT(playInstrumentOnce(Track::Instrument*,int)));
    QObject::connect(&w, SIGNAL(stopInstrument()), &tiaPlayer, SLOT(stopInstrument()));
    QObject::connect(pt, SIGNAL(playWaveform(TiaSound::Distortion,int,int)), &tiaPlayer, SLOT(playWaveform(TiaSound::Distortion,int,int)));
    QObject::connect(&w, SIGNAL(playPercussion(Track::Percussion*)), &tiaPlayer, SLOT(playPercussion(Track::Percussion*)));
    QObject::connect(&w, SIGNAL(stopPercussion()), &tiaPlayer, SLOT(stopPercussion()));
    QObject::connect(&w, SIGNAL(playTrack(int,int)), &tiaPlayer, SLOT(playTrack(int,int)));
    QObject::connect(&w, SIGNAL(stopTrack()), &tiaPlayer, SLOT(stopTrack()));
    Timeline *tl = w.findChild<Timeline *>("trackTimeline");
    PatternEditor *editor = w.findChild<PatternEditor *>("trackEditor");
    QObject::connect(&tiaPlayer, SIGNAL(newPlayerPos(int,int)), tl, SLOT(playerPosChanged(int,int)));
    QObject::connect(&tiaPlayer, SIGNAL(newPlayerPos(int,int)), editor, SLOT(newPlayerPos(int,int)));
    QObject::connect(&tiaPlayer, SIGNAL(invalidNoteFound(int,int,int,QString)), tt, SLOT(invalidNoteFound(int,int,int,QString)));
    QObject::connect(tt, SIGNAL(stopTrack()), &tiaPlayer, SLOT(stopTrack()));
    QObject::connect(editor, SIGNAL(editChannelChanged(int)), &tiaPlayer, SLOT(selectedChannelChanged(int)));
    QCheckBox *cbLoop = w.findChild<QCheckBox *>("checkBoxLoop");
    QObject::connect(cbLoop, SIGNAL(toggled(bool)), &tiaPlayer, SLOT(toggleLoop(bool)));
    QObject::connect(ot, SIGNAL(setTVStandard(TiaSound::TvStandard)), &tiaPlayer, SLOT(setTVStandard(TiaSound::TvStandard)));

    pt->connectPlayer(&tiaPlayer);
    tt->registerPlayer(&tiaPlayer);
    editor->registerPlayer(&tiaPlayer);
    tiaPlayer.run();

    w.updateAllTabs();

    // Shrink window size to minimum and show
    w.resize(0, 0);
    w.show();

    return a.exec();
}
