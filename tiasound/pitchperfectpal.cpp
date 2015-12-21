#include "tiasound.h"
#include "instrumentpitchguide.h"
#include "pitchperfectpal.h"

namespace TiaSound {

InstrumentPitchGuide perfectPalDist1{
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

/*************************************************************************/

InstrumentPitchGuide perfectPalDist2{
    Distortion::BUZZY, "Pitch-perfect PAL Buzzy Rumble", {
        {Note::C_2, +44},
        {Note::C_1, +42},
        {Note::NotANote, +46},
        {Note::NotANote, +44},
    }
};


/*************************************************************************/

InstrumentPitchGuide perfectPalDist3{
    Distortion::BUZZY, "Pitch-perfect PAL Flangy Wavering", {
        {Note::C_2, +44},
        {Note::C_1, +42},
        {Note::NotANote, +46},
        {Note::NotANote, +44},
    }
};


/*************************************************************************/

InstrumentPitchGuide perfectPalDist4{
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

/*************************************************************************/

InstrumentPitchGuide perfectPalDist6{
    Distortion::PURE_HIGH, "Pitch-perfect PAL Pure Low", {
        {Note::H_5, +32},
        {Note::H_4, +32},
        {Note::E_4, +30},
        {Note::H_3, +32},
        {Note::G_3, +45},
        {Note::E_3, +30},
        {Note::D_3, -37},
        {Note::H_2, +32},
        {Note::A_2, +28},
        {Note::G_2, +45},
        {Note::Fis_2, -19},
        {Note::E_2, +31},
        {Note::Dis_2, -9},
        {Note::D_2, -37},
        {Note::C_2, +44},
        {Note::H_1, +32},
        {Note::Ais_1, +26},
        {Note::A_1, +27},
        {Note::Gis_1, +35},
        {Note::G_1, +45},
        {Note::G_1, -39},
        {Note::Fis_1, -20},
        {Note::F_1, +4},
        {Note::E_1, +28},
        {Note::E_1, -39},
        {Note::Dis_1, -9},
        {Note::D_1, +27},
        {Note::D_1, -38},
        {Note::Cis_1, +5},
        {Note::C_1, +42},
        {Note::C_1, -11},
        {Note::NotANote, +33}
    }
};

/*************************************************************************/

InstrumentPitchGuide perfectPalDist7{
    Distortion::PURE_HIGH, "Pitch-perfect PAL Reedy Rumble", {
        {Note::H_5, +32},
        {Note::H_4, +32},
        {Note::E_4, +30},
        {Note::H_3, +32},
        {Note::G_3, +45},
        {Note::E_3, +30},
        {Note::D_3, -37},
        {Note::H_2, +32},
        {Note::A_2, +28},
        {Note::G_2, +45},
        {Note::Fis_2, -19},
        {Note::E_2, +31},
        {Note::Dis_2, -9},
        {Note::D_2, -37},
        {Note::C_2, +44},
        {Note::H_1, +32},
        {Note::Ais_1, +26},
        {Note::A_1, +27},
        {Note::Gis_1, +35},
        {Note::G_1, +45},
        {Note::G_1, -39},
        {Note::Fis_1, -20},
        {Note::F_1, +4},
        {Note::E_1, +28},
        {Note::E_1, -39},
        {Note::Dis_1, -9},
        {Note::D_1, +27},
        {Note::D_1, -38},
        {Note::Cis_1, +5},
        {Note::C_1, +42},
        {Note::C_1, -11},
        {Note::NotANote, +33}
    }
};

/*************************************************************************/

InstrumentPitchGuide perfectPalDist8{
    Distortion::PURE_HIGH, "Pitch-perfect PAL White Noise", {
        {Note::H_1, -18},
        {Note::NotANote, -22},
        {Note::NotANote, -17}
    }
};

/*************************************************************************/

InstrumentPitchGuide perfectPalDist12{
    Distortion::PURE_HIGH, "Pitch-perfect PAL Pure Low", {
        {Note::E_8, -25},
        {Note::E_7, -25},
        {Note::A_6, -27},
        {Note::E_6, -25},
        {Note::C_6, -11},
        {Note::A_5, -27},
        {Note::Fis_5, +7},
        {Note::E_5, -25},
        {Note::D_5, -29},
        {Note::C_5, -11},
        {Note::Ais_4, +23},
        {Note::A_4, 27},
        {Note::G_4, +34},
        {Note::Fis_4, +6},
        {Note::F_4, -13},
        {Note::E_4, -25},
        {Note::Dis_4, -30},
        {Note::D_4, -29},
        {Note::Cis_4, -22},
        {Note::C_4, -11},
        {Note::H_3, +5},
        {Note::Ais_3, +24},
        {Note::A_3, +47},
        {Note::A_3, -27},
        {Note::Gis_3, +2},
        {Note::G_3, +34},
        {Note::G_3, -31},
        {Note::Fis_3, +6},
        {Note::F_3, +45},
        {Note::F_3, -13},
        {Note::E_3, +30},
        {Note::E_3, -25}
    }
};

/*************************************************************************/

InstrumentPitchGuide perfectPalDist14{
    Distortion::PURE_HIGH, "Pitch-perfect PAL Electronic Low", {
        {Note::E_4, +30},
        {Note::E_3, +30},
        {Note::A_2, +28},
        {Note::E_2, +31},
        {Note::C_2, +44},
        {Note::A_1, +27},
        {Note::G_1, -39},
        {Note::E_1, +28},
        {Note::D_1, +27},
        {Note::C_1, +42},
        {Note::NotANote, -22},
        {Note::NotANote, +31},
        {Note::NotANote, -13},
        {Note::NotANote, -36},
        {Note::NotANote, +46},
        {Note::NotANote, +33},
        {Note::NotANote, +25},
        {Note::NotANote, +20},
        {Note::NotANote, +36},
        {Note::NotANote, +44},
        {Note::NotANote, -40}
    }
};

/*************************************************************************/

InstrumentPitchGuide perfectPalDist15{
    Distortion::PURE_HIGH, "Pitch-perfect PAL Electronic High", {
        {Note::E_4, +30},
        {Note::E_3, +30},
        {Note::A_2, +28},
        {Note::E_2, +31},
        {Note::C_2, +44},
        {Note::A_1, +27},
        {Note::G_1, -39},
        {Note::E_1, +28},
        {Note::D_1, +27},
        {Note::C_1, +42},
        {Note::NotANote, -22},
        {Note::NotANote, +31},
        {Note::NotANote, -13},
        {Note::NotANote, -36},
        {Note::NotANote, +46},
        {Note::NotANote, +33},
        {Note::NotANote, +25},
        {Note::NotANote, +20},
        {Note::NotANote, +36},
        {Note::NotANote, +44},
        {Note::NotANote, -40}
    }
};

/*************************************************************************/

InstrumentPitchGuide perfectPalDist16{
    Distortion::PURE_HIGH, "Pitch-perfect PAL Pure Combined", {
        // Dist4 starts here
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
        {Note::H_4, -23},
        // Dist12 starts here
        {Note::E_8, -25},
        {Note::E_7, -25},
        {Note::A_6, -27},
        {Note::E_6, -25},
        {Note::C_6, -11},
        {Note::A_5, -27},
        {Note::Fis_5, +7},
        {Note::E_5, -25},
        {Note::D_5, -29},
        {Note::C_5, -11},
        {Note::Ais_4, +23},
        {Note::A_4, 27},
        {Note::G_4, +34},
        {Note::Fis_4, +6},
        {Note::F_4, -13},
        {Note::E_4, -25},
        {Note::Dis_4, -30},
        {Note::D_4, -29},
        {Note::Cis_4, -22},
        {Note::C_4, -11},
        {Note::H_3, +5},
        {Note::Ais_3, +24},
        {Note::A_3, +47},
        {Note::A_3, -27},
        {Note::Gis_3, +2},
        {Note::G_3, +34},
        {Note::G_3, -31},
        {Note::Fis_3, +6},
        {Note::F_3, +45},
        {Note::F_3, -13},
        {Note::E_3, +30},
        {Note::E_3, -25}
    }
};

}
