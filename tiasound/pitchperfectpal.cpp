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

    const InstrumentPitchGuide perfectPalDist4{
        Distortion::PURE_HIGH, "Pitch-perfect PAL Pure High", {
            {Note::NotANote, 0},
            {Note::H_8, -23},
            {Note::E_8, -25},
            {Note::H_7, -23},
            {Note::G_7, -9},
            {Note::E_7, -25},
            {Note::Cis_7, +8},
            {Note::H_6, -23},
            {Note::A_6, -27},
            {Note::G_6, -9},
            {Note::F_6, +25},
            {Note::E_6, -25},
            {Note::D_6, +36},
            {Note::Cis_6, +8},
            {Note::C_6, -11},
            {Note::H_5, -23},
            {Note::Ais_5, -28},
            {Note::A_5, -27},
            {Note::Gis_5, -20},
            {Note::G_5, -9},
            {Note::Fis_5, +7},
            {Note::F_5, +26},
            {Note::E_5, +48},
            {Note::E_5, -25},
            {Note::Dis_5, +5},
            {Note::D_5, +36},
            {Note::D_5, -29},
            {Note::Cis_5, +8},
            {Note::C_5, +47},
            {Note::C_5, -11},
            {Note::H_4, +32},
            {Note::H_4, -23}
        }
    };
}
