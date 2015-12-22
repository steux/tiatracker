#include "pitchguidefactory.h"
#include "pitchperfectpal.h"

namespace TiaSound {

PitchGuideFactory::PitchGuideFactory()
{
    palGuide.instrumentGuides[Distortion::BUZZY] = perfectPalDist1;
    palGuide.instrumentGuides[Distortion::BUZZY_RUMBLE] = perfectPalDist2;
    palGuide.instrumentGuides[Distortion::FLANGY_WAVERING] = perfectPalDist3;
    palGuide.instrumentGuides[Distortion::PURE_HIGH] = perfectPalDist4;
    palGuide.instrumentGuides[Distortion::PURE_BUZZY] = perfectPalDist6;
    palGuide.instrumentGuides[Distortion::REEDY_RUMBLE] = perfectPalDist7;
    palGuide.instrumentGuides[Distortion::WHITE_NOISE] = perfectPalDist8;
    palGuide.instrumentGuides[Distortion::PURE_LOW] = perfectPalDist12;
    palGuide.instrumentGuides[Distortion::ELECTRONIC_LOW] = perfectPalDist14;
    palGuide.instrumentGuides[Distortion::ELECTRONIC_HIGH] = perfectPalDist15;
    palGuide.instrumentGuides[Distortion::PURE_COMBINED] = perfectPalDist16;
}

/*************************************************************************/

PitchGuide* PitchGuideFactory::getPitchPerfectPalGuide() {
    return &palGuide;
}

/*************************************************************************/

PitchGuide* PitchGuideFactory::getPitchPerfectNtscGuide() {
    return &ntscGuide;
}

}
