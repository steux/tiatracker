/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

#include "pitchguidefactory.h"
#include "pitchperfectpal.h"
#include "percussiontab.h"


namespace TiaSound {

PitchGuideFactory::PitchGuideFactory()
{
    // Generate lists of available frequencies for all TIA distortions
    for (int iDist = 0; iDist < PercussionTab::availableWaveforms.size(); ++iDist) {
        Distortion dist = PercussionTab::availableWaveforms[iDist];
        double divider = distDividers[dist];
        QList<double> palList;
        QList<double> ntscList;
        for (int f = 0; f < 32; ++f) {
            if (dist == Distortion::SILENT) {
                palList.append(0.0);
                ntscList.append(0.0);

            } else {
                palList.append(PalFrequency/divider/(1 + f));
                ntscList.append(NtscFrequency/divider/(1 + f));
            }
        }
        distFrequenciesPal[dist] = palList;
        distFrequenciesNtsc[dist] = ntscList;
    }

/*
    palGuide.instrumentGuides[Distortion::BUZZY] = perfectPalDist1;
    palGuide.instrumentGuides[Distortion::BUZZY_RUMBLE] = perfectPalDist2;
    palGuide.instrumentGuides[Distortion::FLANGY_WAVERING] = perfectPalDist3;
    palGuide.instrumentGuides[Distortion::PURE_HIGH] = perfectPalDist4;
    palGuide.instrumentGuides[Distortion::PURE_BUZZY] = perfectPalDist6;
    palGuide.instrumentGuides[Distortion::REEDY_RUMBLE] = perfectPalDist7;
    palGuide.instrumentGuides[Distortion::WHITE_NOISE] = perfectPalDist8;
    palGuide.instrumentGuides[Distortion::PURE_LOW] = perfectPalDist12;
    palGuide.instrumentGuides[Distortion::ELECTRONIC_RUMBLE] = perfectPalDist14;
    palGuide.instrumentGuides[Distortion::ELECTRONIC_SQUEAL] = perfectPalDist15;
    palGuide.instrumentGuides[Distortion::PURE_COMBINED] = perfectPalDist16;
*/
}

/*************************************************************************/

PitchGuide PitchGuideFactory::getPitchPerfectPalGuide() {
    PitchGuide palGuide{"PAL Pitch-perfect A4=440Hz", TvStandard::PAL};
    for (int iDist = 0; iDist < PercussionTab::availableWaveforms.size(); ++iDist) {
        Distortion dist = PercussionTab::availableWaveforms[iDist];
        QList<FrequencyPitchGuide> guide = calcInstrumentPitchGuide(TvStandard::PAL, dist, 440.0);
        palGuide.instrumentGuides[dist] = InstrumentPitchGuide(dist, palGuide.name, guide);
    }
    // Pure Combined has to be added manually
    QList<FrequencyPitchGuide> combinedGuide = calcInstrumentPitchGuide(TvStandard::PAL, Distortion::PURE_HIGH, 440.0);
    combinedGuide.append(calcInstrumentPitchGuide(TvStandard::PAL, Distortion::PURE_LOW, 440.0));
    palGuide.instrumentGuides[Distortion::PURE_COMBINED] = InstrumentPitchGuide(Distortion::PURE_COMBINED, palGuide.name, combinedGuide);
    return palGuide;
}

/*************************************************************************/

PitchGuide PitchGuideFactory::getPitchPerfectNtscGuide() {
    return ntscGuide;
}

/*************************************************************************/

QList<FrequencyPitchGuide> PitchGuideFactory::calcInstrumentPitchGuide(TiaSound::TvStandard standard, Distortion dist, double baseFreq) {
    QList<FrequencyPitchGuide> fpg;
    // Get nearest notes for all TIA frequencies
    for (int f = 0; f < 32; ++f) {
        double tiaFreq;
        if (standard == TvStandard::PAL) {
            tiaFreq = distFrequenciesPal[dist][f];
        } else {
            tiaFreq = distFrequenciesNtsc[dist][f];
        }
        // Calc minimal distance of this frequency to all "real" notes
        double minPercent = 999999.0;
        int bestNoteIndex = 0;
        for (int n = 0; n < 9*12; ++n) {
            double noteFreq = std::pow(2.0, (n - 49.0)/12.0) * baseFreq;
            double deltaPercent = (std::log(tiaFreq/noteFreq)/std::log(2))*1200;
            if (std::abs(deltaPercent) < std::abs(minPercent)) {
                minPercent = deltaPercent;
                bestNoteIndex = n;
            }
        }
        // Store nearest note if it's close enough
        Note ttNote = Note::NotANote;
        if (minPercent < 50) {
            // For real notes, n=49. In TIATracker, it's 45
            int ttNoteIndex = bestNoteIndex - 4;
            // Only store notes we can display
            if (ttNoteIndex >= 0 && ttNoteIndex < 96) {
                ttNote = getNoteFromInt(ttNoteIndex);
            }
        }
        fpg.append(FrequencyPitchGuide{ttNote, int(std::round(minPercent))});
    }
    return fpg;}

}


