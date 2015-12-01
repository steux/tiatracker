#include "instrumentpitchguide.h"

namespace TiaSound {

/*************************************************************************/

int InstrumentPitchGuide::getNumFrequencies()
{
    return freqPitchGuides.length();
}

/*************************************************************************/

Note InstrumentPitchGuide::getNote(int frequency) {
        return freqPitchGuides[frequency].note;
    }

/*************************************************************************/

int InstrumentPitchGuide::getPercentOff(int frequency) {
        return freqPitchGuides[frequency].percentOff;
    }

}
