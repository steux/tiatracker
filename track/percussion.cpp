#include "percussion.h"
#include <QJsonArray>
#include <QJsonObject>
#include "mainwindow.h"
#include "percussiontab.h"


namespace Track {

/* TODO: More sensible empty detection, like a bool flag that gets
 * changed whenever the percussion data gets updated by the GUI...
 */
bool Percussion::isEmpty() {
    bool empty = true;
    if (name != "---"
            || envelopeLength > 1
            || frequencies.size() > 1
            || waveforms.size() > 1
            || volumes[0] != 0
            || frequencies[0] != 0
            || waveforms[0] != TiaSound::Distortion::WHITE_NOISE
            || overlay
            ) {
        empty = false;
    }

    return empty;
}

/*************************************************************************/

int Percussion::getEnvelopeLength() {
    return envelopeLength;
}

/*************************************************************************/

void Percussion::setEnvelopeLength(int newSize) {
    if (newSize > volumes.size()) {
        // grow
        int lastVolume = volumes[volumes.size() - 1];
        int lastFrequency = frequencies[volumes.size() - 1];
        TiaSound::Distortion lastWaveform = waveforms[volumes.size() - 1];
        while (volumes.size() != newSize) {
            volumes.append(lastVolume);
            frequencies.append(lastFrequency);
            waveforms.append(lastWaveform);
        }
    }
    envelopeLength = newSize;
}

/*************************************************************************/

void Percussion::toJson(QJsonObject &json) {
    json["version"] = MainWindow::version;
    json["name"] = name;
    json["envelopeLength"] = envelopeLength;
    json["overlay"] = overlay;

    QJsonArray freqArray;
    foreach (const int freq, frequencies) {
        freqArray.append(QJsonValue(freq));
    }
    json["frequencies"] = freqArray;

    QJsonArray volArray;
    foreach (const int vol, volumes) {
        volArray.append(QJsonValue(vol));
    }
    json["volumes"] = volArray;

    QJsonArray waveformArray;
    foreach (const TiaSound::Distortion waveform, waveforms) {
        int iWaveform = TiaSound::getDistortionInt(waveform);
        waveformArray.append(QJsonValue(iWaveform));
    }
    json["waveforms"] = waveformArray;

}

/*************************************************************************/

bool Percussion::import(const QJsonObject &json) {
    int version = json["version"].toInt();
    if (version != 1) {
        return false;
    }

    QString newName = json["name"].toString();
    int newEnvelopeLength = json["envelopeLength"].toInt();
    bool newOverlay = json["overlay"].toBool();
    QJsonArray freqArray = json["frequencies"].toArray();
    QJsonArray volArray = json["volumes"].toArray();
    QJsonArray waveformArray = json["waveforms"].toArray();

    // Check for data validity
    if (newName.length() < 1 || newName.length() > PercussionTab::maxPercussionNameLength) {
        return false;
    }
    if (newEnvelopeLength < 2 || newEnvelopeLength > 99) {
        return false;
    }
    if (newEnvelopeLength != freqArray.size() || newEnvelopeLength != volArray.size()
            || newEnvelopeLength != waveformArray.size()) {
        return false;
    }

    // Copy data. Adjust volumes, frequencies or waveforms if necessary.
    frequencies.clear();
    volumes.clear();
    waveforms.clear();
    for (int frame = 0; frame < newEnvelopeLength; ++frame) {
        int newVolume = volArray[frame].toInt();
        if (newVolume < 0) {
            newVolume = 0;
        }
        if (newVolume > 15) {
            newVolume = 15;
        }
        volumes.append(newVolume);
        int newFrequency = freqArray[frame].toInt();
        if (newFrequency < 0) {
            newFrequency = 0;
        }
        if (newFrequency > 31) {
            newFrequency = 31;
        }
        frequencies.append(newFrequency);
        int newWaveform = waveformArray[frame].toInt();
        if (newWaveform < 0 || newWaveform > 15) {
            newWaveform = 0;
        }
        TiaSound::Distortion newDist = TiaSound::distortions[newWaveform];
        waveforms.append(newDist);

    }
    name = newName;
    envelopeLength = newEnvelopeLength;
    overlay = newOverlay;

    return true;
}

/*************************************************************************/

void Percussion::deletePercussion() {
    name = "---";
    while (volumes.size() > 1) {
        volumes.removeLast();
        frequencies.removeLast();
        waveforms.removeLast();
    }
    volumes[0] = 0;
    frequencies[0] = 0;
    waveforms[0] = TiaSound::Distortion::WHITE_NOISE;
    envelopeLength = 1;
    overlay = false;
}

/*************************************************************************/

int Percussion::getMinVolume() {
    int min = volumes[0];
    for (int i = 1; i < volumes.size(); ++i) {
        min = qMin(min, volumes[i]);
    }
    return min;
}

/*************************************************************************/

int Percussion::getMaxVolume() {
    int max = volumes[0];
    for (int i = 1; i < volumes.size(); ++i) {
        max = qMax(max, volumes[i]);
    }
    return max;

}

}
