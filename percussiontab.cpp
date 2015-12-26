#include "percussiontab.h"
#include <QComboBox>
#include <QLineEdit>
#include <cassert>
#include <QLabel>
#include <QSpinBox>
#include "envelopeshaper.h"
#include "percussionshaper.h"
#include <QMessageBox>
#include <iostream>
#include <QFileDialog>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "mainwindow.h"
#include "track/percussion.h"


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
}

/*************************************************************************/

void PercussionTab::updatePercussionTab() {
    assert(pTrack != nullptr);

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

