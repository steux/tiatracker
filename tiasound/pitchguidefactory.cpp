#include "pitchguidefactory.h"
#include "pitchperfectpal.h"

namespace TiaSound {

    PitchGuideFactory::PitchGuideFactory()
    {
        palGuide.instrumentGuides[Distortion::BUZZY] = perfectPalDist1;
    }



    PitchGuide PitchGuideFactory::getPitchPerfectPalGuide() {
        return palGuide;
    }



    PitchGuide PitchGuideFactory::getPitchPerfectNtscGuide() {
        return ntscGuide;
    }

}
