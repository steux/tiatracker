#include "instrument.h"

#include <stdexcept>

namespace Track {

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

}
