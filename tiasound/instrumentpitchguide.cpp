#include "instrumentpitchguide.h"

namespace TiaSound {

/*************************************************************************/

int InstrumentPitchGuide::getNumFrequencies()
{
    return freqPitchGuides.length();
}

/*************************************************************************/

Note InstrumentPitchGuide::getNote(int frequency) {
    if (frequency >= freqPitchGuides.size()) {
        return Note::NotANote;
    } else {
        return freqPitchGuides[frequency].note;
    }
}

/*************************************************************************/

int InstrumentPitchGuide::getPercentOff(int frequency) {
    return freqPitchGuides[frequency].percentOff;
}

}
