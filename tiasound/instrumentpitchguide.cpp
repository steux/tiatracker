#include "instrumentpitchguide.h"

namespace TiaSound {

    InstrumentPitchGuide::InstrumentPitchGuide(Distortion dist, QString name, QList<FrequencyPitchGuide> freqPitchGuides) :
        distortion(dist), name(name), freqPitchGuides(freqPitchGuides)
    {}



    Note InstrumentPitchGuide::getNote(int frequency) {
        return freqPitchGuides[frequency].note;
    }



    int InstrumentPitchGuide::getPercentOff(int frequency) {
        return freqPitchGuides[frequency].percentOff;
    }

}
