#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QString>
#include <QComboBox>

#include "tiasound/tiasound.h"
#include "pianokeyboard.h"
#include "tiasound/pitchperfectpal.h"
#include "waveformshaper.h"
#include "track/track.h"

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

    w.initInstrumentsTab(myTrack);

    /* Initialize GUI elements */
    // PianoKeyboard starting PitchGuide
    PianoKeyboard *pk = w.findChild<PianoKeyboard *>("pianoKeyboard");
    pk->setInstrumentPitchGuide(TiaSound::perfectPalDist1);

    // Volume shaper
    WaveformShaper *vs = w.findChild<WaveformShaper *>("volumeShaper");
    vs->registerTrack(&myTrack);
    vs->name = "Volume";
    vs->setScale(0, 15);
    vs->setValues(myTrack.instruments[0].volumes);

    // Frequency shaper
    WaveformShaper *fs = w.findChild<WaveformShaper *>("frequencyShaper");
    fs->name = "Frequency";
    fs->setScale(-8, 7);
    fs->setValues(myTrack.instruments[0].frequencies);

    // Shrink window size to minimum and show
    w.resize(0, 0);
    w.show();

    return a.exec();
}
