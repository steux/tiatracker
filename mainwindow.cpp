#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "track/instrument.h"


const QColor MainWindow::dark{"#002b36"};
const QColor MainWindow::darkHighlighted{"#073642"};
const QColor MainWindow::light{"#fdf6e3"};
const QColor MainWindow::lightHighlighted{"#eee8d5"};
const QColor MainWindow::contentDark{"#586e75"};
const QColor MainWindow::contentDarker{"#657b83"};
const QColor MainWindow::contentLight{"#839496"};
const QColor MainWindow::contentLighter{"#93a1a1"};
const QColor MainWindow::red{"#dc322f"};
const QColor MainWindow::orange{"#cb4b16"};
const QColor MainWindow::blue{"#268bd2"};



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}



MainWindow::~MainWindow()
{
    delete ui;
}



/**************************************************************************
 * Populate the widgets in the instruments tab with real data from the
 * track.
 *************************************************************************/
void MainWindow::initInstrumentsTab(Track::Track &newTrack)
{
    // Instrument names
    QComboBox *cbInstruments = findChild<QComboBox *>("comboBoxInstruments");
    foreach(Track::Instrument ins, newTrack.instruments) {
        cbInstruments->addItem(ins.name);
    }

    // Instrument waveforms
    QComboBox *cbWaveforms = findChild<QComboBox *>("comboBoxWaveforms");
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
    // Number of waveform frames used
    QLabel *lWaveformsUsed = findChild<QLabel *>("labelWaveformFramesUsed");
    int framesUsed = newTrack.getUsedWaveformFrames();
    lWaveformsUsed->setText("(" + QString::number(framesUsed) + " of 256 used)");
}



/**************************************************************************
 * Instruments tab widget events.
 *************************************************************************/
void MainWindow::on_buttonInstrumentDelete_clicked()
{
    // TODO: Delete
    QComboBox *cb = findChild<QComboBox *>("comboBoxInstruments");
}
