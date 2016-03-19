/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

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
    for (int iFreq = 0; iFreq < envelopeLength; ++iFreq) {
        freqArray.append(QJsonValue(frequencies[iFreq]));
    }
    json["frequencies"] = freqArray;

    QJsonArray volArray;
    for (int iVol = 0; iVol < envelopeLength; ++iVol) {
        volArray.append(QJsonValue(volumes[iVol]));
    }
    json["volumes"] = volArray;

    QJsonArray waveformArray;
    for (int iWave = 0; iWave < envelopeLength; ++iWave) {
        int waveformValue = TiaSound::getDistortionInt(waveforms[iWave]);
        waveformArray.append(QJsonValue(waveformValue));
    }
    json["waveforms"] = waveformArray;

}

/*************************************************************************/

bool Percussion::import(const QJsonObject &json) {
    int version = json["version"].toInt();
    if (version > MainWindow::version) {
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
    if (newEnvelopeLength < 1 || newEnvelopeLength > 99) {
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

/*************************************************************************/

void Percussion::insertFrameBefore(int frame) {
    if (envelopeLength == maxEnvelopeLength) {
        return;
    }
    envelopeLength++;

    // Interpolate
    int volBefore = 0;
    int freqBefore = frequencies[frame];
    if (frame != 0) {
        volBefore = volumes[frame - 1];
        freqBefore = frequencies[frame - 1];
    }
    int volAfter = volumes[frame];
    int newVol = int((volAfter + volBefore)/2);
    int freqAfter = frequencies[frame];
    int freqNew = int((freqAfter + freqBefore)/2);
    TiaSound::Distortion distNew = waveforms[frame];

    volumes.insert(frame, newVol);
    frequencies.insert(frame, freqNew);
    waveforms.insert(frame, distNew);
}

/*************************************************************************/

void Percussion::insertFrameAfter(int frame) {
    if (envelopeLength == maxEnvelopeLength) {
        return;
    }
    envelopeLength++;

    // Interpolate
    int volBefore = volumes[frame];
    int freqBefore = frequencies[frame];
    int volAfter = 0;
    int freqAfter = frequencies[frame];
    // -1 b/c we increased length already
    if (frame + 1 != envelopeLength - 1) {
        volAfter = volumes[frame + 1];
        freqAfter = frequencies[frame + 1];
    }
    int newVol = int((volAfter + volBefore)/2);
    int newFreq = int((freqAfter + freqBefore)/2);
    TiaSound::Distortion distNew = waveforms[frame];

    volumes.insert(frame + 1, newVol);
    frequencies.insert(frame + 1, newFreq);
    waveforms.insert(frame, distNew);
}

/*************************************************************************/

void Percussion::deleteFrame(int frame) {
    if (envelopeLength == 1) {
        return;
    }
    envelopeLength--;

    // Delete frames
    volumes.removeAt(frame);
    frequencies.removeAt(frame);
    waveforms.removeAt(frame);
}

/*************************************************************************/

int Percussion::calcEffectiveSize() {
    int realSize = envelopeLength;
    while (realSize > 0 && waveforms[realSize - 1] == TiaSound::Distortion::SILENT && volumes[realSize - 1] == 0) {
        realSize--;
    }
    return realSize + 1;
}

}
