#include "track.h"
#include <QString>

namespace Track {

    Track::Track()
    {

    }

    int Track::getNumUsedWaveformFrames()
    {
        int numFrames = 0;
        for (int i = 0; i < numInstruments; ++i) {
            numFrames += instruments[i].volumes.size();
        }
        return numFrames;
    }

    int Track::getNumInstruments()
    {
        int usedInstruments = 7;
        for (int i = 0; i < numInstruments; ++i) {
            if (instruments[i].name == "---") {
                usedInstruments--;
            }
        }
        return usedInstruments;
    }

}
