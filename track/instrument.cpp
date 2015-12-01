#include "instrument.h"

#include <stdexcept>
#include <iostream>


namespace Track {


/* TODO: More sensible empty detection, like a bool flag that gets
 * changed whenever the instrument data gets updated by the GUI...
 */
bool Instrument::isEmpty()
{
    bool empty = true;
    if (name != "---"
            || envelopeLength > 2
            || frequencies.size() > 2
            || volumes[0] != 0 || volumes[1] != 0
            || frequencies[0] != 0 || frequencies[1] != 0
            || sustainStart != 0 || releaseStart != 1
            || baseDistortion != TiaSound::Distortion::BUZZY
            ) {
        empty = false;
    }

    return empty;
}


int Instrument::getEnvelopeLength()
{
    return envelopeLength;
}



void Instrument::setEnvelopeLength(int newSize)
{
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



void Instrument::validateSustainReleaseValues()
{
    if (releaseStart >= envelopeLength) {
        releaseStart = envelopeLength - 1;
    }
    if (sustainStart >= releaseStart) {
        sustainStart = releaseStart - 1;
    }
}



void Instrument::setSustainAndRelease(int newSustainStart, int newReleaseStart)
    {
        if (newReleaseStart <= newSustainStart
                || newSustainStart >= envelopeLength - 1
                || newReleaseStart >= envelopeLength) {
            throw std::invalid_argument("Invalid new sustain/release index values!");
        }
        sustainStart = newSustainStart;
        releaseStart = newReleaseStart;
}

int Instrument::getMinVolume()
{
    int min = volumes[0];
    for (int i = 1; i < volumes.size(); ++i) {
        min = qMin(min, volumes[i]);
    }
    return min;
}

int Instrument::getMaxVolume()
{
    int max = volumes[0];
    for (int i = 1; i < volumes.size(); ++i) {
        max = qMax(max, volumes[i]);
    }
    return max;

}

int Instrument::getSustainStart() const
{
    return sustainStart;
}

int Instrument::getReleaseStart() const
{
    return releaseStart;
}

}
