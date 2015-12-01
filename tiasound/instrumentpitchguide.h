#ifndef INSTRUMENTPITCHGUIDE_H
#define INSTRUMENTPITCHGUIDE_H

#include <QString>
#include "tiasound.h"

namespace TiaSound {

/* Pitch guide for a single frequency value */
struct FrequencyPitchGuide {
    Note note;
    int percentOff;
};


/* Frequencies pitch guide for one distortion */
class InstrumentPitchGuide
{
public:
    /* Needed for instantiation inside libraries */
    InstrumentPitchGuide() {}

    /* "Normal" constructor */
    InstrumentPitchGuide(Distortion dist, QString name, QList<FrequencyPitchGuide> freqPitchGuides) :
        distortion(dist), name(name), freqPitchGuides(freqPitchGuides)
    {}

    /* Getters */
    int getNumFrequencies();
    Note getNote(int frequency);
    int getPercentOff(int frequency);

    /* Member variables */
    Distortion distortion;
    QString name;

protected:
    QList<FrequencyPitchGuide> freqPitchGuides;
};

}

#endif // INSTRUMENTPITCHGUIDE_H
