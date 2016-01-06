#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "track/instrument.h"
#include "track/track.h"
#include <QLineEdit>
#include <QSpinBox>
#include <cassert>
#include <QMessageBox>
#include "tiasound/pitchguidefactory.h"
#include "tiasound/pitchguide.h"
#include "tiasound/instrumentpitchguide.h"
#include <QMenu>


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

    // Context menu for envelope widgets    
    waveformContextMenu.addAction(&actionInsertBefore);
    waveformContextMenu.addAction(&actionInsertAfter);
    waveformContextMenu.addAction(&actionDelete);
    ui->volumeShaper->contextMenu = &waveformContextMenu;
    ui->frequencyShaper->contextMenu = &waveformContextMenu;
}

/*************************************************************************/

MainWindow::~MainWindow() {
    delete ui;
}

/*************************************************************************/

void MainWindow::initConnections() {
    // Shaper context menu
    QObject::connect(&actionInsertBefore, SIGNAL(triggered(bool)), this, SLOT(insertFrameBefore(bool)));
    QObject::connect(&actionInsertAfter, SIGNAL(triggered(bool)), this, SLOT(insertFrameAfter(bool)));
    QObject::connect(&actionDelete, SIGNAL(triggered(bool)), this, SLOT(deleteFrame(bool)));

    // InstrumentsTab
    QObject::connect(ui->buttonInstrumentDelete, &QPushButton::clicked, ui->tabInstruments, &InstrumentsTab::on_buttonInstrumentDelete_clicked);
    QObject::connect(ui->buttonInstrumentExport, &QPushButton::clicked, ui->tabInstruments, &InstrumentsTab::on_buttonInstrumentExport_clicked);
    QObject::connect(ui->buttonInstrumentImport, &QPushButton::clicked, ui->tabInstruments, &InstrumentsTab::on_buttonInstrumentImport_clicked);
    QObject::connect(ui->spinBoxInstrumentEnvelopeLength, &QSpinBox::editingFinished, ui->tabInstruments, &InstrumentsTab::on_spinBoxInstrumentEnvelopeLength_editingFinished);
    QObject::connect(ui->spinBoxInstrumentEnvelopeLength, SIGNAL(valueChanged(int)), ui->tabInstruments, SLOT(on_spinBoxInstrumentEnvelopeLength_valueChanged(int)));
    QObject::connect(ui->spinBoxSustainStart, &QSpinBox::editingFinished, ui->tabInstruments, &InstrumentsTab::on_spinBoxSustainStart_editingFinished);
    QObject::connect(ui->spinBoxSustainStart, SIGNAL(valueChanged(int)), ui->tabInstruments, SLOT(on_spinBoxSustainStart_valueChanged(int)));
    QObject::connect(ui->spinBoxReleaseStart, &QSpinBox::editingFinished, ui->tabInstruments, &InstrumentsTab::on_spinBoxReleaseStart_editingFinished);
    QObject::connect(ui->spinBoxReleaseStart, SIGNAL(valueChanged(int)), ui->tabInstruments, SLOT(on_spinBoxReleaseStart_valueChanged(int)));
    QObject::connect(ui->spinBoxInstrumentVolume, &QSpinBox::editingFinished, ui->tabInstruments, &InstrumentsTab::on_spinBoxInstrumentVolume_editingFinished);
    QObject::connect(ui->spinBoxInstrumentVolume, SIGNAL(valueChanged(int)), ui->tabInstruments, SLOT(on_spinBoxInstrumentVolume_valueChanged(int)));
    QObject::connect(ui->comboBoxWaveforms, SIGNAL(currentIndexChanged(int)), ui->tabInstruments, SLOT(on_comboBoxWaveforms_currentIndexChanged(int)));
    QObject::connect(ui->volumeShaper, &EnvelopeShaper::newMaxValue, ui->spinBoxInstrumentVolume, &QSpinBox::setValue);
    QObject::connect(ui->comboBoxInstruments, SIGNAL(currentIndexChanged(int)), ui->tabInstruments, SLOT(on_comboBoxInstruments_currentIndexChanged(int)));
    QObject::connect(ui->comboBoxInstruments, SIGNAL(currentTextChanged(QString)), ui->tabInstruments, SLOT(on_comboBoxInstruments_currentTextChanged(QString)));
    QObject::connect(ui->volumeShaper, SIGNAL(envelopeContextEvent(int)), this, SLOT(waveformContextEvent(int)));
    QObject::connect(ui->frequencyShaper, SIGNAL(envelopeContextEvent(int)), this, SLOT(waveformContextEvent(int)));

    // PercussionTab
    QObject::connect(ui->buttonPercussionDelete, &QPushButton::clicked, ui->tabPercussion, &PercussionTab::on_buttonPercussionDelete_clicked);
    QObject::connect(ui->buttonPercussionExport, &QPushButton::clicked, ui->tabPercussion, &PercussionTab::on_buttonPercussionExport_clicked);
    QObject::connect(ui->buttonPercussionImport, &QPushButton::clicked, ui->tabPercussion, &PercussionTab::on_buttonPercussionImport_clicked);
    QObject::connect(ui->spinBoxPercussionLength, &QSpinBox::editingFinished, ui->tabPercussion, &PercussionTab::on_spinBoxPercussionLength_editingFinished);
    QObject::connect(ui->spinBoxPercussionLength, SIGNAL(valueChanged(int)), ui->tabPercussion, SLOT(on_spinBoxPercussionLength_valueChanged(int)));
    QObject::connect(ui->checkBoxOverlay, SIGNAL(stateChanged(int)), ui->tabPercussion, SLOT(on_checkBoxOverlay_stateChanged(int)));
    QObject::connect(ui->spinBoxPercussionVolume, &QSpinBox::editingFinished, ui->tabPercussion, &PercussionTab::on_spinBoxPercussionVolume_editingFinished);
    QObject::connect(ui->spinBoxPercussionVolume, SIGNAL(valueChanged(int)), ui->tabPercussion, SLOT(on_spinBoxPercussionVolume_valueChanged(int)));
    QObject::connect(ui->percussionVolumeShaper, &PercussionShaper::newMaxValue, ui->spinBoxPercussionVolume, &QSpinBox::setValue);
    QObject::connect(ui->comboBoxPercussion, SIGNAL(currentIndexChanged(int)), ui->tabPercussion, SLOT(on_comboBoxPercussion_currentIndexChanged(int)));
    QObject::connect(ui->comboBoxPercussion, SIGNAL(currentTextChanged(QString)), ui->tabPercussion, SLOT(on_comboBoxPercussion_currentTextChanged(QString)));
    QObject::connect(ui->percussionVolumeShaper, SIGNAL(newPercussionValue(int)), ui->tabPercussion, SLOT(newPercussionValue(int)));
    QObject::connect(ui->percussionFrequencyShaper, SIGNAL(newPercussionValue(int)), ui->tabPercussion, SLOT(newPercussionValue(int)));

    // PianoKeyboard
    QObject::connect(ui->tabInstruments, SIGNAL(setWaveform(TiaSound::Distortion)), this, SLOT(setWaveform(TiaSound::Distortion)));
    QObject::connect(ui->pianoKeyboard, SIGNAL(newKeyPressed(int)), this, SLOT(newPianoKeyPressed(int)));
    QObject::connect(ui->pianoKeyboard, SIGNAL(keyReleased()), this, SLOT(pianoKeyReleased()));
}

/*************************************************************************/

void MainWindow::registerTrack(Track::Track *newTrack) {
    pTrack = newTrack;
}

/*************************************************************************/

void MainWindow::setWaveform(TiaSound::Distortion dist) {
    TiaSound::InstrumentPitchGuide *pIPG = &(pPitchGuide->instrumentGuides[dist]);
    ui->pianoKeyboard->setInstrumentPitchGuide(pIPG);
    ui->pianoKeyboard->update();
}

/*************************************************************************/

void MainWindow::newPianoKeyPressed(int frequency) {
    switch (ui->tabWidget->currentIndex()) {
    case iTabInstruments:
    {
        Track::Instrument *instrument = ui->tabInstruments->getSelectedInstrument();
        emit playInstrument(instrument, frequency);
        break;
    }
    case iTabPercussion:
    {
        Track::Percussion *percussion = ui->tabPercussion->getSelectedPercussion();
        emit playPercussion(percussion);
        break;
    }
    default:
        break;
    }
}

/*************************************************************************/

void MainWindow::pianoKeyReleased() {
    switch (ui->tabWidget->currentIndex()) {
    case iTabInstruments:
        emit stopInstrument();
        break;
    case iTabPercussion:
        break;
    default:
        break;
    }
}

/*************************************************************************/

void MainWindow::on_tabWidget_currentChanged(int index) {
    switch (index) {
    case iTabInstruments:
        ui->pianoKeyboard->setUsePitchGuide(true);
        break;
    case iTabPercussion:
        ui->pianoKeyboard->setUsePitchGuide(false);
        break;
    }
}

/*************************************************************************/

void MainWindow::waveformContextEvent(int frame) {
    waveformContextFrame = frame;
}

/*************************************************************************/

void MainWindow::insertFrameBefore(bool) {
    std::cout << "Insert before " << waveformContextFrame << "\n"; std::cout.flush();
}

/*************************************************************************/

void MainWindow::insertFrameAfter(bool) {

}

/*************************************************************************/

void MainWindow::deleteFrame(bool) {

}
