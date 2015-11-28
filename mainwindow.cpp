#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "track/instrument.h"
#include <QLineEdit>
#include <QSpinBox>
#include <cassert>


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

const QList<TiaSound::Distortion> MainWindow::availableWaveforms{
    TiaSound::Distortion::BUZZY,
    TiaSound::Distortion::BUZZY_RUMBLE,
    TiaSound::Distortion::FLANGY_WAVERING,
    TiaSound::Distortion::PURE_HIGH,
    TiaSound::Distortion::PURE_BUZZY,
    TiaSound::Distortion::REEDY_RUMBLE,
    TiaSound::Distortion::WHITE_NOISE,
    TiaSound::Distortion::PURE_LOW,
    TiaSound::Distortion::ELECTRONIC_LOW,
    TiaSound::Distortion::ELECTRONIC_HIGH
};


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
    /* Global values */
    // Instrument names
    QComboBox *cbInstruments = findChild<QComboBox *>("comboBoxInstruments");
    cbInstruments->lineEdit()->setMaxLength(maxInstrumentNameLength);
    foreach(Track::Instrument ins, newTrack.instruments) {
        cbInstruments->addItem(ins.name);
    }

    // Instrument waveforms
    QComboBox *cbWaveforms = findChild<QComboBox *>("comboBoxWaveforms");
    foreach (TiaSound::Distortion distortion, availableWaveforms) {
        cbWaveforms->addItem(TiaSound::getDistorionName(distortion));
    }
    // Number of envelope frames used
    QLabel *lWaveformsUsed = findChild<QLabel *>("labelWaveformFramesUsed");
    int framesUsed = newTrack.getNumUsedWaveformFrames();
    QString framesUsedString;
    if (framesUsed < 256) {
        framesUsedString = "(" + QString::number(framesUsed) + " of 256 used)";

    } else {
        framesUsedString = "<font color=\"#dc322f\">(" + QString::number(framesUsed) + " of 256 used)</>";
    }
    lWaveformsUsed->setText(framesUsedString);
    // Number of instruments used
    QLabel *lInstrumentsUsed = findChild<QLabel *>("labelInstrumentsUsed");
    int instrumentsUsed = newTrack.getNumInstruments();
    lInstrumentsUsed->setText("(" + QString::number(instrumentsUsed) + " of 7 used)");

    /* Values specific to the selected intrument */
    int iCurInstrument = getSelectedInstrument();
    QLabel *lInstrumentNumber = findChild<QLabel *>("labelInstrumentNumber");
    lInstrumentNumber->setText("Instrument " + QString::number(iCurInstrument + 1));
    Track::Instrument& curInstrument = newTrack.instruments[iCurInstrument];
    // Envelope length
    QSpinBox *spEnvelopeLength = findChild<QSpinBox *>("spinBoxInstrumentEnvelopeLength");
    int envelopeLength = curInstrument.getEnvelopeLength();
    spEnvelopeLength->setValue(envelopeLength);
    // Sustain and release start values
    QSpinBox *spSustainStart = findChild<QSpinBox *>("spinBoxSustainStart");
    int sustainStart = curInstrument.getSustainStart();
    spSustainStart->setValue(sustainStart);
    QSpinBox *spReleaseStart = findChild<QSpinBox *>("spinBoxReleaseStart");
    int releaseStart = curInstrument.getReleaseStart();
    spReleaseStart->setValue(releaseStart);
    // Base waveform
    TiaSound::Distortion curDistortion = curInstrument.baseDistortion;
    int iWaveform = availableWaveforms.indexOf(curDistortion);
    assert(iWaveform != -1);
    cbWaveforms->setCurrentIndex(iWaveform);
}



/**************************************************************************
 * Returns the index of the selected instrument (0..7) in the instruments
 * tab.
 *************************************************************************/
int MainWindow::getSelectedInstrument()
{
    QComboBox *cbInstruments = findChild<QComboBox *>("comboBoxInstruments");
    return cbInstruments->currentIndex();
}



/**************************************************************************
 * Instruments tab widget events.
 *************************************************************************/
void MainWindow::on_buttonInstrumentDelete_clicked()
{
    // TODO: Delete
    //QComboBox *cb = findChild<QComboBox *>("comboBoxInstruments");
}
