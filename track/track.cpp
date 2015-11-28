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
            // If last volume/frequency frames are not 0, add one (0 is mandatory)
            int iLast = instruments[i].volumes.size() - 1;
            if (instruments[i].volumes[iLast] != 0 || instruments[i].frequencies[iLast] != 0) {
                numFrames++;
            }
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
