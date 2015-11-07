#ifndef PITCHGUIDE_H
#define PITCHGUIDE_H

#include <QMap>
#include <QString>

#include "tiasound.h"
#include "instrumentpitchguide.h"

namespace TiaSound {

    class PitchGuide
    {
    public:
        QString name;
        TvStandard tvStandard;
        QMap<Distortion, InstrumentPitchGuide> instrumentGuides;

        PitchGuide(QString name, TvStandard standard) : name(name), tvStandard(standard)
        {}

    protected:
    };

}

#endif // PITCHGUIDE_H
