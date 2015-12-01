#include "track.h"
#include <QString>

namespace Track {

Track::Track() {

}

/*************************************************************************/

int Track::getNumUsedEnvelopeFrames() {
    int numFrames = 0;
    for (int i = 0; i < numInstruments; ++i) {
        if (!instruments[i].isEmpty()) {
            int envelopeLength = instruments[i].getEnvelopeLength();
            numFrames += envelopeLength;
            // If last volume/frequency frames are not 0, add one (0 is mandatory)
            int iLast = envelopeLength - 1;
            if (instruments[i].volumes[iLast] != 0 || instruments[i].frequencies[iLast] != 0) {
                numFrames++;
            }
        }
    }
    return numFrames;
}

/*************************************************************************/

int Track::getNumInstruments() {
    int usedInstruments = numInstruments;
    for (int i = 0; i < numInstruments; ++i) {
        if (instruments[i].isEmpty()) {
            usedInstruments--;
        }
    }
    return usedInstruments;
}

}
