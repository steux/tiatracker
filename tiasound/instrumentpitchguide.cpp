#include "instrumentpitchguide.h"

namespace TiaSound {

    Note InstrumentPitchGuide::getNote(int frequency) {
        return freqPitchGuides[frequency].note;
    }



    int InstrumentPitchGuide::getPercentOff(int frequency) {
        return freqPitchGuides[frequency].percentOff;
    }

}
