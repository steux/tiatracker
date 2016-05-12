/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

#include "pitchguidefactory.h"
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
}

/*************************************************************************/

PitchGuide PitchGuideFactory::getPitchPerfectPalGuide() {

/*
    long bestError = 99999999;
    double bestFreq = 0;
    int bestNum = 0;
    int threshold = 10;
    for (double f = 330.0; f < 660.0; f += 0.1) {
        long currentError = 0;
        int curNum = 0;
        QList<FrequencyPitchGuide> guide = calcInstrumentPitchGuide(TvStandard::NTSC, Distortion::PURE_HIGH, f);
        for (int i = 0; i < guide.size(); ++i) {
            if (std::abs(guide[i].percentOff) <= threshold) {
                currentError += guide[i].percentOff*guide[i].percentOff;
                curNum++;
            }
        }
        if (curNum > bestNum || (curNum == bestNum && currentError < bestError)) {
            std::cout << "Found new best: " << f << ", " << curNum << ": " << currentError << "\n";
            bestError = currentError;
            bestFreq = f;
            bestNum = curNum;
        }
    }
*/

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
    PitchGuide ntscGuide{"NTSC Pitch-perfect A4=440Hz", TvStandard::NTSC};
    for (int iDist = 0; iDist < PercussionTab::availableWaveforms.size(); ++iDist) {
        Distortion dist = PercussionTab::availableWaveforms[iDist];
        QList<FrequencyPitchGuide> guide = calcInstrumentPitchGuide(TvStandard::NTSC, dist, 440.0);
        ntscGuide.instrumentGuides[dist] = InstrumentPitchGuide(dist, ntscGuide.name, guide);
    }
    // Pure Combined has to be added manually
    QList<FrequencyPitchGuide> combinedGuide = calcInstrumentPitchGuide(TvStandard::NTSC, Distortion::PURE_HIGH, 440.0);
    combinedGuide.append(calcInstrumentPitchGuide(TvStandard::NTSC, Distortion::PURE_LOW, 440.0));
    ntscGuide.instrumentGuides[Distortion::PURE_COMBINED] = InstrumentPitchGuide(Distortion::PURE_COMBINED, ntscGuide.name, combinedGuide);
    return ntscGuide;
}

/*************************************************************************/

PitchGuide PitchGuideFactory::getOptimizedPurePalGuide() {
    PitchGuide palGuide{"PAL Optimized Pure A4=345.4Hz", TvStandard::PAL};
    for (int iDist = 0; iDist < PercussionTab::availableWaveforms.size(); ++iDist) {
        Distortion dist = PercussionTab::availableWaveforms[iDist];
        QList<FrequencyPitchGuide> guide = calcInstrumentPitchGuide(TvStandard::PAL, dist, 345.4);
        palGuide.instrumentGuides[dist] = InstrumentPitchGuide(dist, palGuide.name, guide);
    }
    // Pure Combined has to be added manually
    QList<FrequencyPitchGuide> combinedGuide = calcInstrumentPitchGuide(TvStandard::PAL, Distortion::PURE_HIGH, 345.4);
    combinedGuide.append(calcInstrumentPitchGuide(TvStandard::PAL, Distortion::PURE_LOW, 345.4));
    palGuide.instrumentGuides[Distortion::PURE_COMBINED] = InstrumentPitchGuide(Distortion::PURE_COMBINED, palGuide.name, combinedGuide);
    return palGuide;
}

/*************************************************************************/

PitchGuide PitchGuideFactory::getOptimizedPureNtscGuide() {
    PitchGuide ntscGuide{"NTSC Optimized Pure A4=413.9Hz", TvStandard::NTSC};
    for (int iDist = 0; iDist < PercussionTab::availableWaveforms.size(); ++iDist) {
        Distortion dist = PercussionTab::availableWaveforms[iDist];
        QList<FrequencyPitchGuide> guide = calcInstrumentPitchGuide(TvStandard::NTSC, dist, 413.9);
        ntscGuide.instrumentGuides[dist] = InstrumentPitchGuide(dist, ntscGuide.name, guide);
    }
    // Pure Combined has to be added manually
    QList<FrequencyPitchGuide> combinedGuide = calcInstrumentPitchGuide(TvStandard::NTSC, Distortion::PURE_HIGH, 413.9);
    combinedGuide.append(calcInstrumentPitchGuide(TvStandard::NTSC, Distortion::PURE_LOW, 413.9));
    ntscGuide.instrumentGuides[Distortion::PURE_COMBINED] = InstrumentPitchGuide(Distortion::PURE_COMBINED, ntscGuide.name, combinedGuide);
    return ntscGuide;
}

/*************************************************************************/

PitchGuide PitchGuideFactory::getOptimizedDiv31PalGuide() {
    PitchGuide palGuide{"PAL Optimized Flangy/Wavering, Pure Buzzy, White Noise A4=336.5Hz", TvStandard::PAL};
    for (int iDist = 0; iDist < PercussionTab::availableWaveforms.size(); ++iDist) {
        Distortion dist = PercussionTab::availableWaveforms[iDist];
        QList<FrequencyPitchGuide> guide = calcInstrumentPitchGuide(TvStandard::PAL, dist, 336.5);
        palGuide.instrumentGuides[dist] = InstrumentPitchGuide(dist, palGuide.name, guide);
    }
    // Pure Combined has to be added manually
    QList<FrequencyPitchGuide> combinedGuide = calcInstrumentPitchGuide(TvStandard::PAL, Distortion::PURE_HIGH, 336.5);
    combinedGuide.append(calcInstrumentPitchGuide(TvStandard::PAL, Distortion::PURE_LOW, 336.5));
    palGuide.instrumentGuides[Distortion::PURE_COMBINED] = InstrumentPitchGuide(Distortion::PURE_COMBINED, palGuide.name, combinedGuide);
    return palGuide;
}

/*************************************************************************/

PitchGuide PitchGuideFactory::getOptimizedDiv31NtscGuide() {
    PitchGuide ntscGuide{"NTSC Optimized Flangy/Wavering, Pure Buzzy, White Noise A4=339.1Hz", TvStandard::NTSC};
    for (int iDist = 0; iDist < PercussionTab::availableWaveforms.size(); ++iDist) {
        Distortion dist = PercussionTab::availableWaveforms[iDist];
        QList<FrequencyPitchGuide> guide = calcInstrumentPitchGuide(TvStandard::NTSC, dist, 339.1);
        ntscGuide.instrumentGuides[dist] = InstrumentPitchGuide(dist, ntscGuide.name, guide);
    }
    // Pure Combined has to be added manually
    QList<FrequencyPitchGuide> combinedGuide = calcInstrumentPitchGuide(TvStandard::NTSC, Distortion::PURE_HIGH, 339.1);
    combinedGuide.append(calcInstrumentPitchGuide(TvStandard::NTSC, Distortion::PURE_LOW, 339.1));
    ntscGuide.instrumentGuides[Distortion::PURE_COMBINED] = InstrumentPitchGuide(Distortion::PURE_COMBINED, ntscGuide.name, combinedGuide);
    return ntscGuide;
}

/*************************************************************************/

PitchGuide PitchGuideFactory::getOptimizedDiv6PalGuide() {
    PitchGuide palGuide{"PAL Optimized Pure Low, Electronic Aqueal A4=345.0Hz", TvStandard::PAL};
    for (int iDist = 0; iDist < PercussionTab::availableWaveforms.size(); ++iDist) {
        Distortion dist = PercussionTab::availableWaveforms[iDist];
        QList<FrequencyPitchGuide> guide = calcInstrumentPitchGuide(TvStandard::PAL, dist, 345.0);
        palGuide.instrumentGuides[dist] = InstrumentPitchGuide(dist, palGuide.name, guide);
    }
    // Pure Combined has to be added manually
    QList<FrequencyPitchGuide> combinedGuide = calcInstrumentPitchGuide(TvStandard::PAL, Distortion::PURE_HIGH, 345.0);
    combinedGuide.append(calcInstrumentPitchGuide(TvStandard::PAL, Distortion::PURE_LOW, 345.0));
    palGuide.instrumentGuides[Distortion::PURE_COMBINED] = InstrumentPitchGuide(Distortion::PURE_COMBINED, palGuide.name, combinedGuide);
    return palGuide;
}

/*************************************************************************/

PitchGuide PitchGuideFactory::getOptimizedDiv6NtscGuide() {
    PitchGuide ntscGuide{"NTSC Optimized Pure Low, Electronic Aqueal A4=368.3Hz", TvStandard::NTSC};
    for (int iDist = 0; iDist < PercussionTab::availableWaveforms.size(); ++iDist) {
        Distortion dist = PercussionTab::availableWaveforms[iDist];
        QList<FrequencyPitchGuide> guide = calcInstrumentPitchGuide(TvStandard::NTSC, dist, 368.3);
        ntscGuide.instrumentGuides[dist] = InstrumentPitchGuide(dist, ntscGuide.name, guide);
    }
    // Pure Combined has to be added manually
    QList<FrequencyPitchGuide> combinedGuide = calcInstrumentPitchGuide(TvStandard::NTSC, Distortion::PURE_HIGH, 368.3);
    combinedGuide.append(calcInstrumentPitchGuide(TvStandard::NTSC, Distortion::PURE_LOW, 368.3));
    ntscGuide.instrumentGuides[Distortion::PURE_COMBINED] = InstrumentPitchGuide(Distortion::PURE_COMBINED, ntscGuide.name, combinedGuide);
    return ntscGuide;
}

/*************************************************************************/

PitchGuide PitchGuideFactory::getOptimizedDiv2PalGuide() {
    PitchGuide palGuide{"PAL Optimized Pure High, Reedy Rumble A4=345.4Hz", TvStandard::PAL};
    for (int iDist = 0; iDist < PercussionTab::availableWaveforms.size(); ++iDist) {
        Distortion dist = PercussionTab::availableWaveforms[iDist];
        QList<FrequencyPitchGuide> guide = calcInstrumentPitchGuide(TvStandard::PAL, dist, 345.4);
        palGuide.instrumentGuides[dist] = InstrumentPitchGuide(dist, palGuide.name, guide);
    }
    // Pure Combined has to be added manually
    QList<FrequencyPitchGuide> combinedGuide = calcInstrumentPitchGuide(TvStandard::PAL, Distortion::PURE_HIGH, 345.4);
    combinedGuide.append(calcInstrumentPitchGuide(TvStandard::PAL, Distortion::PURE_LOW, 345.4));
    palGuide.instrumentGuides[Distortion::PURE_COMBINED] = InstrumentPitchGuide(Distortion::PURE_COMBINED, palGuide.name, combinedGuide);
    return palGuide;
}

/*************************************************************************/

PitchGuide PitchGuideFactory::getOptimizedDiv2NtscGuide() {
    PitchGuide ntscGuide{"NTSC Optimized Pure High, Reedy Rumble A4=413.9Hz", TvStandard::NTSC};
    for (int iDist = 0; iDist < PercussionTab::availableWaveforms.size(); ++iDist) {
        Distortion dist = PercussionTab::availableWaveforms[iDist];
        QList<FrequencyPitchGuide> guide = calcInstrumentPitchGuide(TvStandard::NTSC, dist, 413.9);
        ntscGuide.instrumentGuides[dist] = InstrumentPitchGuide(dist, ntscGuide.name, guide);
    }
    // Pure Combined has to be added manually
    QList<FrequencyPitchGuide> combinedGuide = calcInstrumentPitchGuide(TvStandard::NTSC, Distortion::PURE_HIGH, 413.9);
    combinedGuide.append(calcInstrumentPitchGuide(TvStandard::NTSC, Distortion::PURE_LOW, 413.9));
    ntscGuide.instrumentGuides[Distortion::PURE_COMBINED] = InstrumentPitchGuide(Distortion::PURE_COMBINED, ntscGuide.name, combinedGuide);
    return ntscGuide;
}

/*************************************************************************/

PitchGuide PitchGuideFactory::calculateGuide(QString name, TvStandard standard, double freq) {
    PitchGuide newGuide{name, standard};
    for (int iDist = 0; iDist < PercussionTab::availableWaveforms.size(); ++iDist) {
        Distortion dist = PercussionTab::availableWaveforms[iDist];
        QList<FrequencyPitchGuide> guide = calcInstrumentPitchGuide(standard, dist, freq);
        newGuide.instrumentGuides[dist] = InstrumentPitchGuide(dist, newGuide.name, guide);
    }
    // Pure Combined has to be added manually
    QList<FrequencyPitchGuide> combinedGuide = calcInstrumentPitchGuide(standard, Distortion::PURE_HIGH, freq);
    combinedGuide.append(calcInstrumentPitchGuide(standard, Distortion::PURE_LOW, freq));
    newGuide.instrumentGuides[Distortion::PURE_COMBINED] = InstrumentPitchGuide(Distortion::PURE_COMBINED, newGuide.name, combinedGuide);
    return newGuide;
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
        } else {
            // So the pitch guide optimizer won't be confused
            minPercent = 0;
        }
        fpg.append(FrequencyPitchGuide{ttNote, int(std::round(minPercent))});
    }
    return fpg;}

}


