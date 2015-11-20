#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QString>
#include <QComboBox>

#include "tiasound/tiasound.h"
#include "pianokeyboard.h"
#include "tiasound/pitchperfectpal.h"
#include "waveformshaper.h"

#include <iostream>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Load and set stylesheet
    QFile styleFile(":/style.qss");
    styleFile.open(QFile::ReadOnly);
    a.setStyleSheet(styleFile.readAll());

    // GUI
    MainWindow w;

    /* Initialize GUI elements */
    // PianoKeyboard starting PitchGuide
    PianoKeyboard *pk = w.findChild<PianoKeyboard *>("pianoKeyboard");
    pk->setInstrumentPitchGuide(TiaSound::perfectPalDist1);

    // Instrument waveforms
    QComboBox *cbWaveforms = w.findChild<QComboBox *>("comboBoxWaveforms");
    cbWaveforms->addItems({
                              TiaSound::getDistorionName(TiaSound::Distortion::BUZZY),
                              TiaSound::getDistorionName(TiaSound::Distortion::BUZZY_RUMBLE),
                              TiaSound::getDistorionName(TiaSound::Distortion::FLANGY_WAVERING),
                              TiaSound::getDistorionName(TiaSound::Distortion::PURE_HIGH),
                              TiaSound::getDistorionName(TiaSound::Distortion::PURE_BUZZY),
                              TiaSound::getDistorionName(TiaSound::Distortion::REEDY_RUMBLE),
                              TiaSound::getDistorionName(TiaSound::Distortion::WHITE_NOISE),
                              TiaSound::getDistorionName(TiaSound::Distortion::PURE_LOW),
                              TiaSound::getDistorionName(TiaSound::Distortion::ELECTRONIC_LOW),
                              TiaSound::getDistorionName(TiaSound::Distortion::ELECTRONIC_HIGH)
                          });

    // Instrument list
    QComboBox *cbInstruments = w.findChild<QComboBox *>("comboBoxInstruments");
    cbInstruments->addItems({
                                "---",
                                "---",
                                "---",
                                "---",
                                "---",
                                "---",
                                "---"
                          });

    // Volume shaper
    WaveformShaper *vs = w.findChild<WaveformShaper *>("volumeShaper");
    vs->name = "Volume";
    vs->scaleMin = 0;
    vs->scaleMax = 15;

    w.show();

    return a.exec();
}
