#include "track.h"
#include <QString>

namespace Track {

    Track::Track()
    {

    }

    int Track::getUsedWaveformFrames()
    {
        int numFrames = 0;
        for (int i = 0; i < 7; ++i) {
            numFrames += instruments[i].volumes.size();
        }
        return numFrames;
    }

}
