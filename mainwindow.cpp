#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "track/instrument.h"
#include "track/track.h"
#include <QLineEdit>
#include <QSpinBox>
#include <cassert>
#include <QMessageBox>


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
const QColor MainWindow::violet{"#6c71c4"};

/*************************************************************************/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

/*************************************************************************/

MainWindow::~MainWindow() {
    delete ui;
}

/*************************************************************************/

void MainWindow::initConnections() {
    // InstrumentsTab
    QObject::connect(ui->buttonInstrumentDelete, &QPushButton::clicked, ui->tabInstruments, &InstrumentsTab::on_buttonInstrumentDelete_clicked);
    QObject::connect(ui->buttonInstrumentExport, &QPushButton::clicked, ui->tabInstruments, &InstrumentsTab::on_buttonInstrumentExport_clicked);
    QObject::connect(ui->spinBoxInstrumentEnvelopeLength, &QSpinBox::editingFinished, ui->tabInstruments, &InstrumentsTab::on_spinBoxInstrumentEnvelopeLength_editingFinished);
    QObject::connect(ui->spinBoxInstrumentEnvelopeLength, SIGNAL(valueChanged(int)), ui->tabInstruments, SLOT(on_spinBoxInstrumentEnvelopeLength_valueChanged(int)));
    QObject::connect(ui->spinBoxSustainStart, &QSpinBox::editingFinished, ui->tabInstruments, &InstrumentsTab::on_spinBoxSustainStart_editingFinished);
    QObject::connect(ui->spinBoxSustainStart, SIGNAL(valueChanged(int)), ui->tabInstruments, SLOT(on_spinBoxSustainStart_valueChanged(int)));
    QObject::connect(ui->spinBoxReleaseStart, &QSpinBox::editingFinished, ui->tabInstruments, &InstrumentsTab::on_spinBoxReleaseStart_editingFinished);
    QObject::connect(ui->spinBoxReleaseStart, SIGNAL(valueChanged(int)), ui->tabInstruments, SLOT(on_spinBoxReleaseStart_valueChanged(int)));
    QObject::connect(ui->spinBoxInstrumentVolume, &QSpinBox::editingFinished, ui->tabInstruments, &InstrumentsTab::on_spinBoxInstrumentVolume_editingFinished);
    QObject::connect(ui->spinBoxInstrumentVolume, SIGNAL(valueChanged(int)), ui->tabInstruments, SLOT(on_spinBoxInstrumentVolume_valueChanged(int)));
    QObject::connect(ui->comboBoxWaveforms, SIGNAL(currentIndexChanged(int)), ui->tabInstruments, SLOT(on_comboBoxWaveforms_currentIndexChanged(int)));
    QObject::connect(ui->volumeShaper, &WaveformShaper::newMaxValue, ui->spinBoxInstrumentVolume, &QSpinBox::setValue);
    QObject::connect(ui->comboBoxInstruments, SIGNAL(currentIndexChanged(int)), ui->tabInstruments, SLOT(on_comboBoxInstruments_currentIndexChanged(int)));
    QObject::connect(ui->comboBoxInstruments, SIGNAL(currentTextChanged(QString)), ui->tabInstruments, SLOT(on_comboBoxInstruments_currentTextChanged(QString)));

    // PianoKeyboard
    QObject::connect(ui->pianoKeyboard, SIGNAL(newKeyPressed(int)), this, SLOT(newPianoKeyPressed(int)));
    QObject::connect(ui->pianoKeyboard, SIGNAL(keyReleased()), this, SLOT(pianoKeyReleased()));
}

/*************************************************************************/

void MainWindow::registerTrack(Track::Track *newTrack) {
    pTrack = newTrack;
}

/*************************************************************************/

void MainWindow::newPianoKeyPressed(int frequency) {
    int instrumentIndex = ui->tabInstruments->getSelectedInstrumentIndex();
    Track::Instrument *instrument = &(pTrack->instruments[instrumentIndex]);
    emit playInstrument(instrument, frequency);
}

/*************************************************************************/

void MainWindow::pianoKeyReleased() {
    emit stopInstrument();
}
