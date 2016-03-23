/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

#ifndef PITCHGUIDEFACTORY_H
#define PITCHGUIDEFACTORY_H

#include <QList>
#include <QString>

#include "pitchguide.h"

namespace TiaSound {

/* Returns PitchGuide objects, which can either be pre-defined ones for
 * PAL or NTSC, or computed ones for a given set of desired notes and
 * distortions.
 */
class PitchGuideFactory
{
public:
    PitchGuideFactory();

    PitchGuide getPitchPerfectPalGuide();
    PitchGuide getPitchPerfectNtscGuide();

private:
    PitchGuide palGuide{"PAL Pitch-perfect A4=440Hz", TvStandard::PAL};
    PitchGuide ntscGuide{"NTSC Pitch-perfect A4=440Hz", TvStandard::NTSC};
};

}

#endif // PITCHGUIDEFACTORY_H
