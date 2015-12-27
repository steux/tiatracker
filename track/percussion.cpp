#include "percussion.h"

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
    // TODO
}

/*************************************************************************/

bool Percussion::import(const QJsonObject &json) {
    // TODO
    return false;
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
