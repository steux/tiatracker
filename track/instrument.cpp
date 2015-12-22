#include "instrument.h"

#include <QMutex>

#include <stdexcept>
#include <iostream>
#include <QJsonArray>
#include "mainwindow.h"


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
