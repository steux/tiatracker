/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

#include "tiasound.h"
#include "instrumentpitchguide.h"
#include "pitchperfectntsc.h"

namespace TiaSound {

InstrumentPitchGuide perfectNtscDist1{
    Distortion::BUZZY, "Pitch-perfect NTSC Buzzy", {
        {Note::C_7, +1},
        {Note::C_6, +1},
        {Note::F_5, 0},
        {Note::C_5, +2},
        {Note::Gis_4, +16},
        {Note::F_4, 0},
        {Note::D_4, +33},
        {Note::C_4, +3},
        {Note::Ais_3, -2},
        {Note::Gis_3, +15},
        {Note::Fis_3, +50},
        {Note::F_3, +1},
        {Note::E_3, -39},
        {Note::D_3, +33},
        {Note::Cis_3, +13},
        {Note::C_3, +3},
        {Note::H_2, -3},
        {Note::Ais_2, 0},
        {Note::A_2, +5},
        {Note::Gis_2, +16},
        {Note::G_2, +31},
        {Note::G_2, -49},
        {Note::Fis_2, -27},
        {Note::F_2, 0},
        {Note::E_2, +29},
        {Note::E_2, -39},
        {Note::Dis_2, -5},
        {Note::D_2, +34},
        {Note::D_2, -27},
        {Note::Cis_2, +15},
        {Note::Cis_2, -44},
        {Note::C_2, +3}
    }
};

/*************************************************************************/

InstrumentPitchGuide perfectNtscDist2{
    Distortion::BUZZY, "Pitch-perfect NTSC Buzzy Rumble", {
        {Note::Cis_2, -44},
        {Note::Cis_1, -42},
        {Note::NotANote, -46},
        {Note::NotANote, -44},
    }
};


/*************************************************************************/

InstrumentPitchGuide perfectNtscDist3{
    Distortion::BUZZY, "Pitch-perfect NTSC Flangy Wavering", {
        {Note::Cis_2, -44},
        {Note::Cis_1, -42},
        {Note::NotANote, -46},
        {Note::NotANote, -44},
    }
};


/*************************************************************************/

InstrumentPitchGuide perfectNtscDist4{
    Distortion::PURE_HIGH, "Pitch-perfect NTSC Pure High", {
        {Note::NotANote, 0},
        {Note::H_8, -9},
        {Note::E_8, -11},
        {Note::H_7, -10},
        {Note::G_7, +4},
        {Note::E_7, -11},
        {Note::Cis_7, +21},
        {Note::H_6, -9},
        {Note::A_6, -13},
        {Note::G_6, +4},
        {Note::F_6, +39},
        {Note::E_6, -11},
        {Note::D_6, +49},
        {Note::Cis_6, +22},
        {Note::C_6, +2},
        {Note::H_5, -10},
        {Note::Ais_5, -15},
        {Note::A_5, -14},
        {Note::Gis_5, -7},
        {Note::G_5, +4},
        {Note::Fis_5, +20},
        {Note::F_5, +39},
        {Note::F_5, -38},
        {Note::E_5, -12},
        {Note::Dis_5, +18},
        {Note::D_5, +49},
        {Note::D_5, -16},
        {Note::Cis_5, +21},
        {Note::Cis_5, -40},
        {Note::C_5, +2},
        {Note::H_4, +45},
        {Note::H_4, -9}
    }
};

/*************************************************************************/

InstrumentPitchGuide perfectNtscDist6{
    Distortion::PURE_HIGH, "Pitch-perfect NTSC Pure Low", {
        {Note::H_5, +45},
        {Note::H_4, +45},
        {Note::E_4, +43},
        {Note::H_3, +45},
        {Note::Gis_3, -42},
        {Note::E_3, +43},
        {Note::D_3, -23},
        {Note::H_2, +42},
        {Note::A_2, +42},
        {Note::Gis_2, -41},
        {Note::Fis_2, -6},
        {Note::E_2, +43},
        {Note::Dis_2, +4},
        {Note::D_2, -24},
        {Note::Cis_2, -44},
        {Note::H_1, +46},
        {Note::Ais_1, +41},
        {Note::A_1, +39},
        {Note::Gis_1, +48},
        {Note::Gis_1, -41},
        {Note::G_1, -25},
        {Note::Fis_1, -4},
        {Note::F_1, +16},
        {Note::E_1, +44},
        {Note::E_1, -26},
        {Note::Dis_1, +4},
        {Note::D_1, +41},
        {Note::D_1, -24},
        {Note::Cis_1, +19},
        {Note::Cis_1, -42},
        {Note::C_1, 0},
        {Note::NotANote, +44}
    }
};

/*************************************************************************/

InstrumentPitchGuide perfectNtscDist7{
    Distortion::PURE_HIGH, "Pitch-perfect NTSC Reedy Rumble", {
        {Note::H_5, +45},
        {Note::H_4, +45},
        {Note::E_4, +43},
        {Note::H_3, +45},
        {Note::Gis_3, -42},
        {Note::E_3, +43},
        {Note::D_3, -23},
        {Note::H_2, +42},
        {Note::A_2, +42},
        {Note::Gis_2, -41},
        {Note::Fis_2, -6},
        {Note::E_2, +43},
        {Note::Dis_2, +4},
        {Note::D_2, -24},
        {Note::Cis_2, -44},
        {Note::H_1, +46},
        {Note::Ais_1, +41},
        {Note::A_1, +39},
        {Note::Gis_1, +48},
        {Note::Gis_1, -41},
        {Note::G_1, -25},
        {Note::Fis_1, -4},
        {Note::F_1, +16},
        {Note::E_1, +44},
        {Note::E_1, -26},
        {Note::Dis_1, +4},
        {Note::D_1, +41},
        {Note::D_1, -24},
        {Note::Cis_1, +19},
        {Note::Cis_1, -42},
        {Note::C_1, 0},
        {Note::NotANote, +44}
    }
};

/*************************************************************************/

InstrumentPitchGuide perfectNtscDist8{
    Distortion::PURE_HIGH, "Pitch-perfect NTSC White Noise", {
        {Note::H_1, -6},
        {Note::NotANote, -6},
        {Note::NotANote, -8}
    }
};

/*************************************************************************/

InstrumentPitchGuide perfectNtscDist12{
    Distortion::PURE_HIGH, "Pitch-perfect NTSC Pure Low", {
        {Note::E_8, -11},
        {Note::E_7, -11},
        {Note::A_6, -14},
        {Note::E_6, -11},
        {Note::C_6, +2},
        {Note::A_5, -14},
        {Note::Fis_5, +20},
        {Note::E_5, -12},
        {Note::D_5, -16},
        {Note::C_5, +2},
        {Note::Ais_4, +39},
        {Note::A_4, -13},
        {Note::G_4, +48},
        {Note::Fis_4, +20},
        {Note::F_4, 0},
        {Note::E_4, -11},
        {Note::Dis_4, -17},
        {Note::D_4, -16},
        {Note::Cis_4, -9},
        {Note::C_4, +3},
        {Note::H_3, +18},
        {Note::Ais_3, +37},
        {Note::Ais_3, -40},
        {Note::A_3, -14},
        {Note::Gis_3, +15},
        {Note::G_3, +47},
        {Note::G_3, -17},
        {Note::Fis_3, +19},
        {Note::Fis_3, -41},
        {Note::F_3, +1},
        {Note::E_3, +43},
        {Note::E_3, -11}
    }
};

/*************************************************************************/

InstrumentPitchGuide perfectNtscDist14{
    Distortion::PURE_HIGH, "Pitch-perfect NTSC Electronic Low", {
        {Note::E_4, +43},
        {Note::E_3, +43},
        {Note::A_2, +42},
        {Note::E_2, +43},
        {Note::Cis_2, -44},
        {Note::A_1, +39},
        {Note::G_1, -25},
        {Note::E_1, +44},
        {Note::D_1, +41},
        {Note::Cis_1, -42},
        {Note::NotANote, -11},
        {Note::NotANote, +44},
        {Note::NotANote, 0},
        {Note::NotANote, -29},
        {Note::NotANote, -46},
        {Note::NotANote, +42},
        {Note::NotANote, +42},
        {Note::NotANote, +40},
        {Note::NotANote, +45},
        {Note::NotANote, -44},
        {Note::NotANote, -30}
    }
};

/*************************************************************************/

InstrumentPitchGuide perfectNtscDist15{
    Distortion::PURE_HIGH, "Pitch-perfect NTSC Electronic High", {
        {Note::E_4, +43},
        {Note::E_3, +43},
        {Note::A_2, +42},
        {Note::E_2, +43},
        {Note::Cis_2, -44},
        {Note::A_1, +39},
        {Note::G_1, -25},
        {Note::E_1, +44},
        {Note::D_1, +41},
        {Note::Cis_1, -42},
        {Note::NotANote, -11},
        {Note::NotANote, +44},
        {Note::NotANote, 0},
        {Note::NotANote, -29},
        {Note::NotANote, -46},
        {Note::NotANote, +42},
        {Note::NotANote, +42},
        {Note::NotANote, +40},
        {Note::NotANote, +45},
        {Note::NotANote, -44},
        {Note::NotANote, -30}
    }
};

/*************************************************************************/

InstrumentPitchGuide perfectNtscDist16{
    Distortion::PURE_HIGH, "Pitch-perfect NTSC Pure Combined", {
        // Dist4 starts here
        {Note::NotANote, 0},
        {Note::H_8, -9},
        {Note::E_8, -11},
        {Note::H_7, -10},
        {Note::G_7, +4},
        {Note::E_7, -11},
        {Note::Cis_7, +21},
        {Note::H_6, -9},
        {Note::A_6, -13},
        {Note::G_6, +4},
        {Note::F_6, +39},
        {Note::E_6, -11},
        {Note::D_6, +49},
        {Note::Cis_6, +22},
        {Note::C_6, +2},
        {Note::H_5, -10},
        {Note::Ais_5, -15},
        {Note::A_5, -14},
        {Note::Gis_5, -7},
        {Note::G_5, +4},
        {Note::Fis_5, +20},
        {Note::F_5, +39},
        {Note::F_5, -38},
        {Note::E_5, -12},
        {Note::Dis_5, +18},
        {Note::D_5, +49},
        {Note::D_5, -16},
        {Note::Cis_5, +21},
        {Note::Cis_5, -40},
        {Note::C_5, +2},
        {Note::H_4, +45},
        {Note::H_4, -9},
        // Dist12 starts here
        {Note::E_8, -11},
        {Note::E_7, -11},
        {Note::A_6, -14},
        {Note::E_6, -11},
        {Note::C_6, +2},
        {Note::A_5, -14},
        {Note::Fis_5, +20},
        {Note::E_5, -12},
        {Note::D_5, -16},
        {Note::C_5, +2},
        {Note::Ais_4, +39},
        {Note::A_4, -13},
        {Note::G_4, +48},
        {Note::Fis_4, +20},
        {Note::F_4, 0},
        {Note::E_4, -11},
        {Note::Dis_4, -17},
        {Note::D_4, -16},
        {Note::Cis_4, -9},
        {Note::C_4, +3},
        {Note::H_3, +18},
        {Note::Ais_3, +37},
        {Note::Ais_3, -40},
        {Note::A_3, -14},
        {Note::Gis_3, +15},
        {Note::G_3, +47},
        {Note::G_3, -17},
        {Note::Fis_3, +19},
        {Note::Fis_3, -41},
        {Note::F_3, +1},
        {Note::E_3, +43},
        {Note::E_3, -11}
    }
};

}
