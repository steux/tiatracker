#ifndef INSTRUMENTPITCHGUIDE_H
#define INSTRUMENTPITCHGUIDE_H

#include <QString>
#include "tiasound.h"

namespace TiaSound {

    /* Pitch guide for one frequency value
     */
    struct FrequencyPitchGuide {
        Note note;
        int percentOff;
    };



    /* Frequencies pitch guide for one distortion
     */
    class InstrumentPitchGuide
    {
    public:
        Distortion distortion;
        QString name;

        InstrumentPitchGuide() {}

        InstrumentPitchGuide(Distortion dist, QString name, QList<FrequencyPitchGuide> freqPitchGuides) :
            distortion(dist), name(name), freqPitchGuides(freqPitchGuides)
        {}

        int getNumFrequencies();
        Note getNote(int frequency);
        int getPercentOff(int frequency);

    protected:
        QList<FrequencyPitchGuide> freqPitchGuides;
    };

}

#endif // INSTRUMENTPITCHGUIDE_H
