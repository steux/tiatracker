#ifndef TIASOUND_H
#define TIASOUND_H

#include <QString>
#include <QList>

namespace TiaSound {

    /* Note names for use as index
     */
    enum class Note : int {
        C_1 = 0, Cis_1, D_1, Dis_1, E_1, F_1, Fis_1, G_1, Gis_1, A_1, Ais_1, H_1,
        C_2, Cis_2, D_2, Dis_2, E_2, F_2, Fis_2, G_2, Gis_2, A_2, Ais_2, H_2,
        C_3, Cis_3, D_3, Dis_3, E_3, F_3, Fis_3, G_3, Gis_3, A_3, Ais_3, H_3,
        C_4, Cis_4, D_4, Dis_4, E_4, F_4, Fis_4, G_4, Gis_4, A_4, Ais_4, H_4,
        C_5, Cis_5, D_5, Dis_5, E_5, F_5, Fis_5, G_5, Gis_5, A_5, Ais_5, H_5,
        C_6, Cis_6, D_6, Dis_6, E_6, F_6, Fis_6, G_6, Gis_6, A_6, Ais_6, H_6,
        C_7, Cis_7, D_7, Dis_7, E_7, F_7, Fis_7, G_7, Gis_7, A_7, Ais_7, H_7,
        NotANote
    };

    /* Get a note name as QString from note index, without octave number */
    QString getNoteName(Note note);
    /* Get a note name as QString from note index, with octave number */
    QString getNoteNameWithOctave(Note note);



    /* Distortions
     */
    enum class Distortion : int {
        BUZZY=1,
        BUZZY_RUMBLE=2,
        FLANGY_WAVERING=3,
        PURE_HIGH=4,
        PURE_BUZZY=6,
        REEDY_RUMBLE=7,
        WHITE_NOISE=8,
        PURE_LOW=12,
        ELECTRONIC_LOW=14,
        ELECTRONIC_HIGH=15
    };

    /* Get a distortion name as QString from Distortion index
     */
    QString getDistorionName(Distortion dist);


    /* TV Standards
     */
    enum class TvStandard {
        PAL, NTSC
    };

}

#endif // TIASOUND_H
