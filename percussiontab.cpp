#include "percussiontab.h"
#include <QComboBox>
#include <QLineEdit>
#include <cassert>
#include <QLabel>
#include <QSpinBox>
#include "envelopeshaper.h"
#include <QMessageBox>
#include <iostream>
#include <QFileDialog>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "mainwindow.h"


const QList<TiaSound::Distortion> PercussionTab::availableWaveforms{
    TiaSound::Distortion::SILENT,
    TiaSound::Distortion::BUZZY,
    TiaSound::Distortion::BUZZY_RUMBLE,
    TiaSound::Distortion::FLANGY_WAVERING,
    TiaSound::Distortion::PURE_HIGH,
    TiaSound::Distortion::PURE_BUZZY,
    TiaSound::Distortion::REEDY_RUMBLE,
    TiaSound::Distortion::WHITE_NOISE,
    TiaSound::Distortion::PURE_LOW,
    TiaSound::Distortion::ELECTRONIC_RUMBLE,
    TiaSound::Distortion::ELECTRONIC_SQUEAL
};

/*************************************************************************/

PercussionTab::PercussionTab(QWidget *parent) : QWidget(parent)
{

}

/*************************************************************************/

void PercussionTab::registerTrack(Track::Track *newTrack) {
    pTrack = newTrack;
}

/*************************************************************************/

void PercussionTab::initPercussionTab() {
    // Percussion names
    QComboBox *cbPercussion = findChild<QComboBox *>("comboBoxPercussion");
    assert (cbPercussion != nullptr);

    cbPercussion->lineEdit()->setMaxLength(maxPercussionNameLength);
//    foreach(Track::Instrument ins, pTrack->instruments) {
//        cbInstruments->addItem(ins.name);
//    }

    // Volume shaper
    EnvelopeShaper *vs = findChild<EnvelopeShaper *>("percussionVolumeShaper");
    assert (vs != nullptr);

    vs->registerInstrument(&(pTrack->instruments[0]));
    vs->name = "Volume";
    vs->setScale(0, 15);
    vs->setValues(&(pTrack->instruments[0].volumes));

    // Frequency shaper
    EnvelopeShaper *fs = findChild<EnvelopeShaper *>("percussionFrequencyShaper");
    assert (fs != nullptr);

    fs->registerInstrument(&(pTrack->instruments[0]));
    fs->name = "Frequency";
    fs->setScale(0, 31);
    fs->isInverted = true;
    fs->setValues(&(pTrack->instruments[0].frequencies));
}

/*************************************************************************/

void PercussionTab::updatePercussionTab() {
    assert(pTrack != nullptr);

}

/*************************************************************************/

int PercussionTab::getSelectedPercussionIndex() {
    // TODO
}

/*************************************************************************/

