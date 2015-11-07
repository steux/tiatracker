#ifndef PITCHGUIDEFACTORY_H
#define PITCHGUIDEFACTORY_H

#include <QList>
#include <QString>

#include "pitchguide.h"

namespace TiaSound {

    class PitchGuideFactory
    {
    public:
        PitchGuideFactory();

        PitchGuide getPitchPerfectPalGuide();
        PitchGuide getPitchPerfectNtscGuide();

    private:

        PitchGuide palGuide{"Pitch-perfect PAL", TvStandard::PAL};
        PitchGuide ntscGuide{"Pitch-perfect NTSC", TvStandard::NTSC};
    };

}

#endif // PITCHGUIDEFACTORY_H
