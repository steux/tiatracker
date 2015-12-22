#include "instrumentstab.h"
#include <QComboBox>
#include <QLineEdit>
#include <cassert>
#include <QLabel>
#include <QSpinBox>
#include "waveformshaper.h"
#include <QMessageBox>
#include <iostream>
#include <QFileDialog>
#include <QStringList>


const QList<TiaSound::Distortion> InstrumentsTab::availableWaveforms{
    TiaSound::Distortion::BUZZY,
    TiaSound::Distortion::BUZZY_RUMBLE,
    TiaSound::Distortion::FLANGY_WAVERING,
    TiaSound::Distortion::PURE_HIGH,
    TiaSound::Distortion::PURE_BUZZY,
    TiaSound::Distortion::REEDY_RUMBLE,
    TiaSound::Distortion::WHITE_NOISE,
    TiaSound::Distortion::PURE_LOW,
    TiaSound::Distortion::ELECTRONIC_LOW,
    TiaSound::Distortion::ELECTRONIC_HIGH,
    TiaSound::Distortion::PURE_COMBINED
};

/*************************************************************************/

InstrumentsTab::InstrumentsTab(QWidget *parent) : QWidget(parent)
{

}

/*************************************************************************/

void InstrumentsTab::registerTrack(Track::Track *newTrack) {
    pTrack = newTrack;
}

/*************************************************************************/

void InstrumentsTab::initInstrumentsTab() {
    // Instrument names
    QComboBox *cbInstruments = findChild<QComboBox *>("comboBoxInstruments");
    cbInstruments->lineEdit()->setMaxLength(maxInstrumentNameLength);
    foreach(Track::Instrument ins, pTrack->instruments) {
        cbInstruments->addItem(ins.name);
    }
    // Instrument waveforms
    QComboBox *cbWaveforms = findChild<QComboBox *>("comboBoxWaveforms");
    foreach (TiaSound::Distortion distortion, availableWaveforms) {
        cbWaveforms->addItem(TiaSound::getDistorionName(distortion));
    }
    cbWaveforms->setCurrentIndex(10);   // Init to PURE_COMBINED

    // Volume shaper
    WaveformShaper *vs = findChild<WaveformShaper *>("volumeShaper");
    vs->registerInstrument(&(pTrack->instruments[0]));
    vs->name = "Volume";
    vs->setScale(0, 15);
    vs->setValues(&(pTrack->instruments[0].volumes));

    // Frequency shaper
    WaveformShaper *fs = findChild<WaveformShaper *>("frequencyShaper");
    fs->registerInstrument(&(pTrack->instruments[0]));
    fs->name = "Frequency";
    fs->setScale(-8, 7);
    fs->isInverted = true;
    fs->setValues(&(pTrack->instruments[0].frequencies));
}

/*************************************************************************/

void InstrumentsTab::updateInstrumentsTab() {
    assert(pTrack != nullptr);

    /* Global values */
    // Number of envelope frames used
    QLabel *lWaveformsUsed = findChild<QLabel *>("labelWaveformFramesUsed");
    int framesUsed = pTrack->getNumUsedEnvelopeFrames();
    QString framesUsedString;
    if (framesUsed < 256) {
        framesUsedString = "(" + QString::number(framesUsed) + " of 256 used)";

    } else {
        framesUsedString = "<font color=\"#dc322f\">(" + QString::number(framesUsed) + " of 256 used)</>";
    }
    lWaveformsUsed->setText(framesUsedString);
    // Number of instruments used
    QLabel *lInstrumentsUsed = findChild<QLabel *>("labelInstrumentsUsed");
    int instrumentsUsed = pTrack->getNumInstruments();
    QString instrumentsUsedString;
    if (instrumentsUsed <= 7) {
        instrumentsUsedString = "(" + QString::number(instrumentsUsed) + " of 7 used)";
    } else {
        instrumentsUsedString = "<font color=\"#dc322f\">(" + QString::number(instrumentsUsed) + " of 7 used)</>";
    }
    lInstrumentsUsed->setText(instrumentsUsedString);

    /* Values specific to the selected intrument */
    int iCurInstrument = getSelectedInstrumentIndex();
    QLabel *lInstrumentNumber = findChild<QLabel *>("labelInstrumentNumber");
    lInstrumentNumber->setText("Instrument " + QString::number(iCurInstrument + 1));
    Track::Instrument& curInstrument = pTrack->instruments[iCurInstrument];
    // Envelope length
    QSpinBox *spEnvelopeLength = findChild<QSpinBox *>("spinBoxInstrumentEnvelopeLength");
    int envelopeLength = curInstrument.getEnvelopeLength();
    spEnvelopeLength->setValue(envelopeLength);
    // Sustain and release start values
    QSpinBox *spSustainStart = findChild<QSpinBox *>("spinBoxSustainStart");
    int sustainStart = curInstrument.getSustainStart();
    spSustainStart->setValue(sustainStart + 1);
    QSpinBox *spReleaseStart = findChild<QSpinBox *>("spinBoxReleaseStart");
    int releaseStart = curInstrument.getReleaseStart();
    spReleaseStart->setValue(releaseStart + 1);
    // Peak volume
    QSpinBox *spPeakVolume = findChild<QSpinBox *>("spinBoxInstrumentVolume");
    int maxVolume = curInstrument.getMaxVolume();
    spPeakVolume->setValue(maxVolume);
    // Base waveform
    TiaSound::Distortion curDistortion = curInstrument.baseDistortion;
    int iWaveform = availableWaveforms.indexOf(curDistortion);
    assert(iWaveform != -1);
    QComboBox *cbWaveforms = findChild<QComboBox *>("comboBoxWaveforms");
    cbWaveforms->setCurrentIndex(iWaveform);
    emit setWaveform(curInstrument.baseDistortion);
    // WaveformShaper sizes and values
    WaveformShaper *wsVolume = findChild<WaveformShaper *>("volumeShaper");
    wsVolume->registerInstrument(&curInstrument);
    wsVolume->setValues(&(curInstrument.volumes));
    wsVolume->updateSize();
    WaveformShaper *wsFrequency = findChild<WaveformShaper *>("frequencyShaper");
    wsFrequency->registerInstrument(&curInstrument);
    wsFrequency->setValues(&(curInstrument.frequencies));
    wsFrequency->updateSize();
}

/*************************************************************************/

int InstrumentsTab::getSelectedInstrumentIndex() {
    QComboBox *cbInstruments = findChild<QComboBox *>("comboBoxInstruments");
    return cbInstruments->currentIndex();
}

/*************************************************************************/

Track::Instrument * InstrumentsTab::getSelectedInstrument() {
    int iCurInstrument = getSelectedInstrumentIndex();
    Track::Instrument *curInstrument = &(pTrack->instruments[iCurInstrument]);
    return curInstrument;
}

/*************************************************************************/

void InstrumentsTab::on_buttonInstrumentDelete_clicked() {
    Track::Instrument *curInstrument = getSelectedInstrument();
    bool doDelete = true;
    if (!curInstrument->isEmpty()) {
        QMessageBox msgBox(QMessageBox::NoIcon,
                           "Delete Instrument",
                           "Do you really want to delete this instument?",
                           QMessageBox::Yes | QMessageBox::No, this,
                           Qt::FramelessWindowHint);
        int reply = msgBox.exec();
        if (reply != QMessageBox::Yes) {
            doDelete = false;
        }
    }
    if (doDelete) {
        pTrack->lock();
        curInstrument->deleteInstrument();
        pTrack->unlock();
        updateInstrumentsTab();
        update();
    }
}

/*************************************************************************/

void InstrumentsTab::on_buttonInstrumentExport_clicked() {
    Track::Instrument *curInstrument = getSelectedInstrument();
    if (curInstrument->isEmpty()) {
        return;
    }

    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("*.tti");
    dialog.setDefaultSuffix("tti");
    dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
    }
}

/*************************************************************************/

void InstrumentsTab::on_spinBoxInstrumentEnvelopeLength_editingFinished() {
    QSpinBox *sb = findChild<QSpinBox *>("spinBoxInstrumentEnvelopeLength");
    int newLength = sb->value();
    Track::Instrument *curInstrument = getSelectedInstrument();
    pTrack->lock();
    curInstrument->setEnvelopeLength(newLength);
    pTrack->unlock();
    updateInstrumentsTab();
    update();
}

void InstrumentsTab::on_spinBoxInstrumentEnvelopeLength_valueChanged(int newLength) {
    Track::Instrument *curInstrument = getSelectedInstrument();
    if (std::abs(newLength - curInstrument->getEnvelopeLength()) == 1) {
        on_spinBoxInstrumentEnvelopeLength_editingFinished();
    }
}

/*************************************************************************/

void InstrumentsTab::on_spinBoxSustainStart_editingFinished() {
    QSpinBox *sb = findChild<QSpinBox *>("spinBoxSustainStart");
    int newStart = sb->value() - 1;
    Track::Instrument *curInstrument = getSelectedInstrument();
    if (newStart < curInstrument->getReleaseStart()) {
        // valid new value
        curInstrument->setSustainAndRelease(newStart, curInstrument->getReleaseStart());
    } else {
        // invalid new value. Try to push release start
        int newRelease = newStart + 1;
        if (newRelease < curInstrument->getEnvelopeLength()) {
            curInstrument->setSustainAndRelease(newStart, newRelease);
        } else {
            // Release start cannot be pushed ahead, so reject new sustain value
            sb->setValue(curInstrument->getSustainStart() + 1);
        }
    }
    updateInstrumentsTab();
    update();
}

void InstrumentsTab::on_spinBoxSustainStart_valueChanged(int newStart) {
    newStart--;
    Track::Instrument *curInstrument = getSelectedInstrument();
    if (std::abs(newStart - curInstrument->getSustainStart()) == 1) {
        on_spinBoxSustainStart_editingFinished();
    }
}

/*************************************************************************/

void InstrumentsTab::on_spinBoxReleaseStart_editingFinished() {
    QSpinBox *sb = findChild<QSpinBox *>("spinBoxReleaseStart");
    int newStart = sb->value() - 1;
    Track::Instrument *curInstrument = getSelectedInstrument();
    if (newStart < curInstrument->getEnvelopeLength()
            && newStart > curInstrument->getSustainStart()) {
        // valid new value
        curInstrument->setSustainAndRelease(curInstrument->getSustainStart(), newStart);
    } else {
        // invalid new value
        sb->setValue(curInstrument->getReleaseStart() + 1);
    }
    updateInstrumentsTab();
    update();
}

void InstrumentsTab::on_spinBoxReleaseStart_valueChanged(int newStart) {
    newStart--;
    Track::Instrument *curInstrument = getSelectedInstrument();
    if (std::abs(newStart - curInstrument->getReleaseStart()) == 1) {
        on_spinBoxReleaseStart_editingFinished();
    }
}

/*************************************************************************/

void InstrumentsTab::on_spinBoxInstrumentVolume_editingFinished() {
    QSpinBox *sb = findChild<QSpinBox *>("spinBoxInstrumentVolume");
    int newVolume = sb->value();
    Track::Instrument *curInstrument = getSelectedInstrument();
    int curMin = curInstrument->getMinVolume();
    int curMax = curInstrument->getMaxVolume();
    int curVolumeSpan = curMax - curMin;
    if (newVolume - curVolumeSpan >= 0) {
        // Shift volumes
        int volumeShift = newVolume - curMax;
        for (int i = 0; i < curInstrument->getEnvelopeLength(); ++i) {
            curInstrument->volumes[i] += volumeShift;
        }
    } else {
        // Invalid value: Set volume to current max
        sb->setValue(curInstrument->getMaxVolume());
    }
    updateInstrumentsTab();
    update();
}

void InstrumentsTab::on_spinBoxInstrumentVolume_valueChanged(int newVolume) {
    Track::Instrument *curInstrument = getSelectedInstrument();
    if (std::abs(newVolume - curInstrument->getMaxVolume()) == 1) {
        on_spinBoxInstrumentVolume_editingFinished();
    }
}

/*************************************************************************/

void InstrumentsTab::on_comboBoxWaveforms_currentIndexChanged(int index) {
    Track::Instrument *curInstrument = getSelectedInstrument();
    TiaSound::Distortion newDistortion = availableWaveforms[index];
    curInstrument->baseDistortion = newDistortion;

    updateInstrumentsTab();
    update();
}

/*************************************************************************/

void InstrumentsTab::on_comboBoxInstruments_currentIndexChanged(int) {
    updateInstrumentsTab();
    update();
}

void InstrumentsTab::on_comboBoxInstruments_currentTextChanged(const QString &text) {
    Track::Instrument *curInstrument = getSelectedInstrument();
    curInstrument->name = text;
    updateInstrumentsTab();
    update();
}
