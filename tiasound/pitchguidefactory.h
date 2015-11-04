#ifndef PITCHGUIDEFACTORY_H
#define PITCHGUIDEFACTORY_H

#include "pitchguide.h"

namespace TiaSound {

    class PitchGuideFactory
    {
    public:
        PitchGuideFactory();

        PitchGuide getPitchPerfectPalGuide();
        PitchGuide getPitchPerfectNtscGuide();

    private:
        PitchGuide palGuide;
        PitchGuide ntscGuide;
    };

}

#endif // PITCHGUIDEFACTORY_H
