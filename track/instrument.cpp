#include "instrument.h"

#include <stdexcept>

namespace Track {


/* TODO: More sensible empty detection, like a bool flag that gets
 * changed whenever the instrument data gets updated by the GUI...
 */
bool Instrument::isEmpty()
{
    bool empty = true;
    if (name != "---"
            || volumes.size() > 2
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
    return volumes.size();
}



void Instrument::changeEnvelopeLength(int newSize)
{
    if (newSize > getEnvelopeLength()) {
        // grow
        int lastVolume = volumes[volumes.size() - 1];
        int lastFrequency = frequencies[volumes.size() - 1];
        while (volumes.size() != newSize) {
            volumes.append(lastVolume);
            frequencies.append(lastFrequency);
        }
    } else {
        // shrink
        while (volumes.size() != newSize) {
            volumes.removeLast();
            frequencies.removeLast();
        }
    }
    validateSustainReleaseValues();
}



void Instrument::validateSustainReleaseValues()
{
    if (releaseStart >= getEnvelopeLength()) {
        releaseStart = getEnvelopeLength() - 1;
    }
    if (sustainStart >= releaseStart) {
        sustainStart = releaseStart - 1;
    }
}



void Instrument::setSustainAndRelease(int newSustainStart, int newReleaseStart)
    {
        if (newReleaseStart <= newSustainStart
                || newSustainStart >= volumes.size() - 1
                || newReleaseStart >= volumes.size()) {
            throw std::invalid_argument("Invalid new sustain/release index values!");
        }
        sustainStart = newSustainStart;
        releaseStart = newReleaseStart;
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
