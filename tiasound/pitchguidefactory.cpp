#include "pitchguidefactory.h"

namespace TiaSound {

    PitchGuideFactory::PitchGuideFactory()
    {

    }



    PitchGuide PitchGuideFactory::getPitchPerfectPalGuide() {
        return palGuide;
    }



    PitchGuide PitchGuideFactory::getPitchPerfectNtscGuide() {
        return ntscGuide;
    }

}
