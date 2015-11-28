#include "instrument.h"

#include <stdexcept>

namespace Track {

bool Instrument::isEmpty()
{
    bool empty = true;
    if (name != "---"
            || volumes.size() > 2
            || frequencies.size() > 2
            || volumes[0] != 0 || volumes[1] != 0
            || frequencies[0] != 0 || frequencies[1] != 0) {
        empty = false;
    }

    return empty;
}


int Instrument::getEnvelopeLength()
{
     return volumes.size();
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
