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
    static const int numNotes = 7*12;

    PitchGuideFactory();

    PitchGuide getPitchPerfectPalGuide();
    PitchGuide getPitchPerfectNtscGuide();
    PitchGuide getOptimizedPurePalGuide();
    PitchGuide getOptimizedPureNtscGuide();
    PitchGuide getOptimizedDiv31PalGuide();
    PitchGuide getOptimizedDiv31NtscGuide();
    PitchGuide getOptimizedDiv6PalGuide();
    PitchGuide getOptimizedDiv6NtscGuide();
    PitchGuide getOptimizedDiv2PalGuide();
    PitchGuide getOptimizedDiv2NtscGuide();

    PitchGuide calculateGuide(QString name, TvStandard standard, double freq);

    QList<FrequencyPitchGuide> calcInstrumentPitchGuide(TvStandard standard, Distortion dist, double baseFreq);

private:
    static const int PalFrequency = 31200;
    static const int NtscFrequency = 31440;

    const QMap<Distortion, double> distDividers{
        {Distortion::SILENT, 999999.9},
        {Distortion::BUZZY, 15.0},              // AUDC 1
        {Distortion::BUZZY_RUMBLE, 232.5},      // AUDC 2   Stolberg: 465
        {Distortion::FLANGY_WAVERING, 31.0},    // AUDC 3   Stolberg: 465
        {Distortion::PURE_HIGH, 2},             // AUDC 4
        {Distortion::PURE_BUZZY, 31.0},         // AUDC 6
        {Distortion::REEDY_RUMBLE, 31.0},       // AUDC 7   Stolberg: 2
        {Distortion::WHITE_NOISE, 31.0},        // AUDC 8   Stolberg: 511
        {Distortion::PURE_LOW, 6.0},            // AUDC 12
        {Distortion::ELECTRONIC_RUMBLE, 93.0},  // AUDC 14
        {Distortion::ELECTRONIC_SQUEAL, 31.0}   // AUDC 15  Stolberg: 6
    };

    QMap<Distortion, QList<double>> distFrequenciesPal;
    QMap<Distortion, QList<double>> distFrequenciesNtsc;

};

}

#endif // PITCHGUIDEFACTORY_H
