#include "tiasound.h"
#include "instrumentpitchguide.h"
#include "pitchperfectpal.h"

namespace TiaSound {

    const InstrumentPitchGuide perfectPalDist1{
        Distortion::BUZZY, "Pitch-perfect PAL Buzzy", {
            {Note::C_7, -1},
            {Note::C_6, -1},
            {Note::F_5, -1},
            {Note::C_5, -1},
            {Note::Gis_4, +3},
            {Note::F_4, -13},
            {Note::D_4, +20},
            {Note::C_4, -11},
            {Note::Ais_3, -15},
            {Note::Gis_3, +2},
            {Note::Fis_3, +37},
            {Note::F_3, -13},
            {Note::Dis_3, +48},
            {Note::D_3, +20},
            {Note::Cis_3, +1},
            {Note::C_3, -11},
            {Note::H_2, -16},
            {Note::Ais_2, -14},
            {Note::A_2, -8},
            {Note::Gis_2, +3},
            {Note::G_2, +17},
            {Note::Fis_2, +36},
            {Note::Fis_2, -40},
            {Note::F_2, -12},
            {Note::E_2, +16},
            {Note::Dis_2, +48},
            {Note::Dis_2, -18},
            {Note::D_2, +20},
            {Note::D_2, -41},
            {Note::Cis_2, 0},
            {Note::C_2, +44},
            {Note::C_2, -11}
        }
    };

}
