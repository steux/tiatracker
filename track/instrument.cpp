#include "instrument.h"

#include <QMutex>

#include <stdexcept>
#include <iostream>
#include <QJsonArray>
#include <QJsonObject>
#include "mainwindow.h"
#include "instrumentstab.h"


namespace Track {

/* TODO: More sensible empty detection, like a bool flag that gets
 * changed whenever the instrument data gets updated by the GUI...
 */
bool Instrument::isEmpty() {
    bool empty = true;
    if (name != "---"
            || envelopeLength > 2
            || frequencies.size() > 2
            || volumes[0] != 0 || volumes[1] != 0
            || frequencies[0] != 0 || frequencies[1] != 0
            || sustainStart != 0 || releaseStart != 1
            || baseDistortion != TiaSound::Distortion::PURE_COMBINED
            ) {
        empty = false;
    }

    return empty;
}

/*************************************************************************/

void Instrument::toJson(QJsonObject &json) {
    json["version"] = MainWindow::version;
    json["name"] = name;
    json["waveform"] = static_cast<int>(baseDistortion);
    json["envelopeLength"] = envelopeLength;
    json["sustainStart"] = sustainStart;
    json["releaseStart"] = releaseStart;

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
}

/*************************************************************************/

bool Instrument::import(const QJsonObject &json) {
    int version = json["version"].toInt();
    if (version != 1) {
        return false;
    }

    QString newName = json["name"].toString();
    int newWaveform = json["waveform"].toInt();
    int newEnvelopeLength = json["envelopeLength"].toInt();
    int newSustainStart = json["sustainStart"].toInt();
    int newReleaseStart = json["releaseStart"].toInt();
    QJsonArray freqArray = json["frequencies"].toArray();
    QJsonArray volArray = json["volumes"].toArray();

    // Check for data validity
    if (newName.length() < 1 || newName.length() > InstrumentsTab::maxInstrumentNameLength) {
        return false;
    }
    if (newWaveform < 0 || newWaveform > 16) {
        return false;
    }
    if (newEnvelopeLength < 2 || newEnvelopeLength > 99
            || newSustainStart < 0 || newSustainStart >= newEnvelopeLength - 1
            || newReleaseStart <= newSustainStart || newReleaseStart > newEnvelopeLength - 1) {
        return false;
    }
    if (newEnvelopeLength != freqArray.size() || newEnvelopeLength != volArray.size()) {
        return false;
    }

    // Copy data. Adjust volumes or frequencies if necessary.
    frequencies.clear();
    volumes.clear();
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
        if (newFrequency < -8) {
            newFrequency = -8;
        }
        if (newFrequency > 7) {
            newFrequency = 7;
        }
        frequencies.append(newFrequency);
    }
    name = newName;
    baseDistortion = TiaSound::distortions[newWaveform];
    envelopeLength = newEnvelopeLength;
    sustainStart = newSustainStart;
    releaseStart = newReleaseStart;

    return true;
}

/*************************************************************************/

void Instrument::setEnvelopeLength(int newSize) {
    if (newSize > volumes.size()) {
        // grow
        int lastVolume = volumes[volumes.size() - 1];
        int lastFrequency = frequencies[volumes.size() - 1];
        while (volumes.size() != newSize) {
            volumes.append(lastVolume);
            frequencies.append(lastFrequency);
        }
    }
    envelopeLength = newSize;
    validateSustainReleaseValues();
}

/*************************************************************************/

int Instrument::getAudCValue(int frequency) {
    int result;

    if (baseDistortion != TiaSound::Distortion::PURE_COMBINED) {
        result = (static_cast<int>(baseDistortion));
    } else {
        if (frequency < 32) {
            result = (static_cast<int>(TiaSound::Distortion::PURE_HIGH));
        } else {
            result = (static_cast<int>(TiaSound::Distortion::PURE_LOW));
        }
    }

    return result;
}

/*************************************************************************/

void Instrument::validateSustainReleaseValues() {
    if (releaseStart >= envelopeLength) {
        releaseStart = envelopeLength - 1;
    }
    if (sustainStart >= releaseStart) {
        sustainStart = releaseStart - 1;
    }
}

/*************************************************************************/

void Instrument::setSustainAndRelease(int newSustainStart, int newReleaseStart) {
    // TODO: change intro assert
    if (newReleaseStart <= newSustainStart
            || newSustainStart >= envelopeLength - 1
            || newReleaseStart >= envelopeLength) {
        throw std::invalid_argument("Invalid new sustain/release index values!");
    }
    sustainStart = newSustainStart;
    releaseStart = newReleaseStart;
}

/*************************************************************************/

void Instrument::deleteInstrument()
{
    name = "---";
    while (volumes.size() > 2) {
        volumes.removeLast();
        frequencies.removeLast();
    }
    volumes[0] = 0;
    volumes[1] = 0;
    frequencies[0] = 0;
    frequencies[1] = 0;
    envelopeLength = 2;
    sustainStart = 0;
    releaseStart = 1;
    baseDistortion = TiaSound::Distortion::PURE_COMBINED;
}

/*************************************************************************/

int Instrument::getMinVolume() {
    int min = volumes[0];
    for (int i = 1; i < volumes.size(); ++i) {
        min = qMin(min, volumes[i]);
    }
    return min;
}

/*************************************************************************/

int Instrument::getMaxVolume() {
    int max = volumes[0];
    for (int i = 1; i < volumes.size(); ++i) {
        max = qMax(max, volumes[i]);
    }
    return max;
}

/*************************************************************************/

void Instrument::insertFrameBefore(int frame) {
    if (envelopeLength == maxEnvelopeLength) {
        return;
    }
    envelopeLength++;

    // Correct sustain and release ranges
    if (frame < sustainStart) {
        // in AD
        sustainStart++;
        releaseStart++;
    } else if (frame < releaseStart) {
        // in Sustain
        releaseStart++;
    }

    // Interpolate volume
    int volBefore = 0;
    if (frame != 0) {
        volBefore = volumes[frame - 1];
    }
    int volAfter = volumes[frame];
    int newVol = int((volAfter + volBefore)/2 + 0.5);
    volumes.insert(frame, newVol);

    // Interpolate frequency
    int freqBefore = 0;
    if (frame != 0) {
        freqBefore = frequencies[frame - 1];
    }
    int freqAfter = frequencies[frame];
    int newFreq = int((freqAfter + freqBefore)/2 + 0.5);
    frequencies.insert(frame, newFreq);
}

/*************************************************************************/

int Instrument::getSustainStart() const {
    return sustainStart;
}

int Instrument::getReleaseStart() const {
    return releaseStart;
}

/*************************************************************************/

int Instrument::getEnvelopeLength() {
    return envelopeLength;
}

}
