#include "percussiontab.h"
#include <QComboBox>
#include <QLineEdit>
#include <cassert>
#include <QLabel>
#include <QSpinBox>
#include "envelopeshaper.h"
#include "percussionshaper.h"
#include "waveformshaper.h"
#include <QMessageBox>
#include <iostream>
#include <QFileDialog>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "mainwindow.h"
#include "track/percussion.h"
#include <QCheckBox>


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
    foreach(Track::Percussion perc, pTrack->percussion) {
        cbPercussion->addItem(perc.name);
    }

    // Volume shaper
    PercussionShaper *vs = findChild<PercussionShaper *>("percussionVolumeShaper");
    assert (vs != nullptr);
    vs->registerPercussion(&(pTrack->percussion[0]));
    vs->name = "Volume";
    vs->setScale(0, 15);
    vs->setValues(&(pTrack->percussion[0].volumes));

    // Frequency shaper
    PercussionShaper *fs = findChild<PercussionShaper *>("percussionFrequencyShaper");
    assert (fs != nullptr);
    fs->registerPercussion(&(pTrack->percussion[0]));
    fs->name = "Frequency";
    fs->setScale(0, 31);
    fs->isInverted = true;
    fs->setValues(&(pTrack->percussion[0].frequencies));

    // Waveform shaper
    WaveformShaper *ws = findChild<WaveformShaper *>("percussionWaveformShaper");
    assert (ws != nullptr);
    ws->registerPercussion(&(pTrack->percussion[0]));
    ws->setValues(&(pTrack->percussion[0].waveforms));
}

/*************************************************************************/

void PercussionTab::updatePercussionTab() {
    assert(pTrack != nullptr);
    /* Global values */
    // Number of envelope frames used
    QLabel *lFramesUsed = findChild<QLabel *>("labelPercussionFramesUsed");
    int framesUsed = pTrack->getNumUsedPercussionFrames();
    QString framesUsedString;
    if (framesUsed < 256) {
        framesUsedString = "(" + QString::number(framesUsed) + " of 256 used)";

    } else {
        framesUsedString = "<font color=\"#dc322f\">(" + QString::number(framesUsed) + " of 256 used)</>";
    }
    lFramesUsed->setText(framesUsedString);
    // Number of percussion used
    QLabel *lPercussionUsed = findChild<QLabel *>("labelPercussionUsed");
    int percussionUsed = pTrack->getNumPercussion();
    QString percussionUsedString = "(" + QString::number(percussionUsed) + " of 15 used)";
    lPercussionUsed->setText(percussionUsedString);

    /* Values specific to the selected percussion */
    int iCurPercussion = getSelectedPercussionIndex();
    QLabel *lPercussionNumber = findChild<QLabel *>("labelPercussionNumber");
    lPercussionNumber->setText("Percussion " + QString::number(iCurPercussion + 1));
    Track::Percussion& curPercussion = pTrack->percussion[iCurPercussion];
    // Envelope length
    QSpinBox *spPercussionLength = findChild<QSpinBox *>("spinBoxPercussionLength");
    int envelopeLength = curPercussion.getEnvelopeLength();
    spPercussionLength->setValue(envelopeLength);
    // Peak volume
    QSpinBox *spPeakVolume = findChild<QSpinBox *>("spinBoxPercussionVolume");
    int maxVolume = curPercussion.getMaxVolume();
    spPeakVolume->setValue(maxVolume);
    // Overlay
    QCheckBox *cpOverlay = findChild<QCheckBox *>("checkBoxOverlay");
    cpOverlay->setChecked(curPercussion.overlay);
    // PercussionShaper sizes and values
    PercussionShaper *psVolume = findChild<PercussionShaper *>("percussionVolumeShaper");
    psVolume->registerPercussion(&curPercussion);
    psVolume->setValues(&(curPercussion.volumes));
    psVolume->updateSize();
    PercussionShaper *psFrequency = findChild<PercussionShaper *>("percussionFrequencyShaper");
    psFrequency->registerPercussion(&curPercussion);
    psFrequency->setValues(&(curPercussion.frequencies));
    psFrequency->updateSize();
    WaveformShaper *wsWaveforms = findChild<WaveformShaper *>("percussionWaveformShaper");
    wsWaveforms->registerPercussion(&curPercussion);
    wsWaveforms->setValues(&(curPercussion.waveforms));
    wsWaveforms->updateSize();
}

/*************************************************************************/

int PercussionTab::getSelectedPercussionIndex() {
    QComboBox *cbPercussion = findChild<QComboBox *>("comboBoxPercussion");
    return cbPercussion->currentIndex();
}

/*************************************************************************/

Track::Percussion *PercussionTab::getSelectedPercussion() {
    int iCurPercussion = getSelectedPercussionIndex();
    Track::Percussion *curPercussion = &(pTrack->percussion[iCurPercussion]);
    return curPercussion;
}

/*************************************************************************/

void PercussionTab::on_buttonPercussionDelete_clicked() {
    Track::Percussion *curPercussion = getSelectedPercussion();
    bool doDelete = true;
    if (!curPercussion->isEmpty()) {
        QMessageBox msgBox(QMessageBox::NoIcon,
                           "Delete Percussion",
                           "Do you really want to delete this percussion?",
                           QMessageBox::Yes | QMessageBox::No, this,
                           Qt::FramelessWindowHint);
        int reply = msgBox.exec();
        if (reply != QMessageBox::Yes) {
            doDelete = false;
        }
    }
    if (doDelete) {
        pTrack->lock();
        curPercussion->deletePercussion();
        pTrack->unlock();
        updatePercussionTab();
        update();
    }
}

/*************************************************************************/

void PercussionTab::on_spinBoxPercussionLength_editingFinished() {
    QSpinBox *sb = findChild<QSpinBox *>("spinBoxPercussionLength");
    int newLength = sb->value();
    Track::Percussion *curPercussion = getSelectedPercussion();
    pTrack->lock();
    curPercussion->setEnvelopeLength(newLength);
    pTrack->unlock();
    updatePercussionTab();
    update();
}

/*************************************************************************/

void PercussionTab::on_spinBoxPercussionLength_valueChanged(int newLength) {
    Track::Percussion *curPercussion = getSelectedPercussion();
    if (std::abs(newLength - curPercussion->getEnvelopeLength()) == 1) {
        on_spinBoxPercussionLength_editingFinished();
    }
}

/*************************************************************************/

void PercussionTab::on_checkBoxOverlay_stateChanged(int) {
    QCheckBox *cpOverlay = findChild<QCheckBox *>("checkBoxOverlay");
    Track::Percussion *curPercussion = getSelectedPercussion();
    curPercussion->overlay = cpOverlay->isChecked();

}

/*************************************************************************/

void PercussionTab::on_spinBoxPercussionVolume_editingFinished() {
    QSpinBox *sb = findChild<QSpinBox *>("spinBoxPercussionVolume");
    int newVolume = sb->value();
    Track::Percussion *curPercussion = getSelectedPercussion();
    int curMin = curPercussion->getMinVolume();
    int curMax = curPercussion->getMaxVolume();
    int curVolumeSpan = curMax - curMin;
    if (newVolume - curVolumeSpan >= 0) {
        // Shift volumes
        int volumeShift = newVolume - curMax;
        for (int i = 0; i < curPercussion->getEnvelopeLength(); ++i) {
            curPercussion->volumes[i] += volumeShift;
        }
    } else {
        // Invalid value: Set volume to current max
        sb->setValue(curPercussion->getMaxVolume());
    }
    updatePercussionTab();
    update();
}

/*************************************************************************/

void PercussionTab::on_spinBoxPercussionVolume_valueChanged(int newVolume) {
    Track::Percussion *curPercussion = getSelectedPercussion();
    if (std::abs(newVolume - curPercussion->getMaxVolume()) == 1) {
        on_spinBoxPercussionVolume_editingFinished();
    }
}

/*************************************************************************/

void PercussionTab::on_comboBoxPercussion_currentIndexChanged(int) {
    updatePercussionTab();
    update();
}

/*************************************************************************/

void PercussionTab::on_comboBoxPercussion_currentTextChanged(const QString &text) {
    Track::Percussion *curPercussion = getSelectedPercussion();
    curPercussion->name = text;
    updatePercussionTab();
    update();
}

/*************************************************************************/

