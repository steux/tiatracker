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
#include <QMap>

#include "pitchguide.h"
#include "tiasound.h"

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

    QList<FrequencyPitchGuide> calcInstrumentPitchGuide(TvStandard standard, Distortion dist, double baseFreq);

private:
    static const int PalFrequency = 31200;
    static const int NtscFrequency = 31440;

    const QMap<Distortion, double> distDividers{
        {Distortion::SILENT, 999999.9},
        {Distortion::BUZZY, 15.0},
        {Distortion::BUZZY_RUMBLE, 232.5},      // Eckhard: 465
        {Distortion::FLANGY_WAVERING, 31.0},    // Eckhard: 465
        {Distortion::PURE_HIGH, 2},
        {Distortion::PURE_BUZZY, 31.0},
        {Distortion::REEDY_RUMBLE, 2.0},
        {Distortion::WHITE_NOISE, 31.0},        // Eckhard: 511
        {Distortion::PURE_LOW, 6.0},
        {Distortion::ELECTRONIC_RUMBLE, 93.0},
        {Distortion::ELECTRONIC_SQUEAL, 6.0}
    };

    QMap<Distortion, QList<double>> distFrequenciesPal;
    QMap<Distortion, QList<double>> distFrequenciesNtsc;

    //PitchGuide palGuide{"PAL Pitch-perfect A4=440Hz", TvStandard::PAL};
    PitchGuide ntscGuide{"NTSC Pitch-perfect A4=440Hz", TvStandard::NTSC};

};

}

#endif // PITCHGUIDEFACTORY_H
