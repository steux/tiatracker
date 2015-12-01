#ifndef PITCHGUIDE_H
#define PITCHGUIDE_H

#include <QMap>
#include <QString>

#include "tiasound.h"
#include "instrumentpitchguide.h"

namespace TiaSound {

/* A pitch guide for a given TV standard that maps frequencies to nearest
 * notes and percent values for how off the notes are. The QMap
 * instrumentGuides holds these mappings per distortion value.
 */
class PitchGuide
{
public:
    PitchGuide(QString name, TvStandard standard)
        : name(name), tvStandard(standard)
    {}

    QString name;
    TvStandard tvStandard;
    QMap<Distortion, InstrumentPitchGuide> instrumentGuides;

};

}

#endif // PITCHGUIDE_H
