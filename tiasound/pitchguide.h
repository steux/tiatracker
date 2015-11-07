#ifndef PITCHGUIDE_H
#define PITCHGUIDE_H

#include <QHash>
#include <QString>

#include "tiasound.h"
#include "instrumentpitchguide.h"

namespace TiaSound {

    class PitchGuide
    {
    public:
        QString name;
        TvStandard tvStandard;
        QHash<Distortion, InstrumentPitchGuide> instrumentGuides;

        PitchGuide(QString name, TvStandard standard) : name(name), tvStandard(standard)
        {}

    protected:
    };

}

#endif // PITCHGUIDE_H
