#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QString>
#include <QComboBox>
#include <QThread>

#include "tiasound/tiasound.h"
#include "pianokeyboard.h"
#include "tiasound/pitchperfectpal.h"
#include "waveformshaper.h"
#include "track/track.h"
#include "instrumentstab.h"
#include "emulation/player.h"

#include <iostream>



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

    InstrumentsTab *it = w.findChild<InstrumentsTab *>("tabInstruments");
    it->registerTrack(&myTrack);
    it->initInstrumentsTab();
    it->updateInstrumentsTab();

    w.initConnections();

    /* Initialize GUI elements */
    // PianoKeyboard starting PitchGuide
    PianoKeyboard *pk = w.findChild<PianoKeyboard *>("pianoKeyboard");
    pk->setInstrumentPitchGuide(TiaSound::perfectPalDist1);

    /* Create and initialize player thread */
    Emulation::Player tiaPlayer;
    w.connectPlayer(&tiaPlayer);
    tiaPlayer.run();

    // Shrink window size to minimum and show
    w.resize(0, 0);
    w.show();




    return a.exec();
}
