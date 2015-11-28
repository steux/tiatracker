#include "instrument.h"

#include <stdexcept>

namespace Track {

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
