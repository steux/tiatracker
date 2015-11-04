#include "tiasound.h"

namespace TiaSound {

    QString getNoteName(Note note) {
        static const QList<QString> noteNames{{"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "H"}};

        int noteIndexInOctave = (static_cast<int>(note))%12;
        return noteNames[noteIndexInOctave];
    }



    QString getNoteNameWithOctave(Note note)
    {
        static const QList<QString> noteNames{{"C%1", "C#%1", "D%1", "D#%1", "E%1", "F%1", "F#%1", "G%1", "G#%1", "A%1", "A#%1", "H%1"}};

        int noteIndexInOctave = (static_cast<int>(note))%12;
        int octave = int((static_cast<int>(note))/12);

        return noteNames[noteIndexInOctave].arg(octave);
    }



    QString getDistorionName(Distortion dist)
    {
        static const QList<QString> distNames{{
                "Silent",           // 0
                "Buzzy",            // 1
                "Buzzy/Rumble",     // 2
                "Flangy/Wavering",  // 3
                "Pure High",        // 4
                "Pure High",        // 5
                "Pure Buzzy",       // 6
                "Reedy/Rumble",     // 7
                "White Noise",      // 8
                "Reedy/Rumble",     // 9
                "Pure Buzzy",       // 10
                "Silent",           // 11
                "Pure Low",         // 12
                "Pure Low",         // 13
                "Electronic Low",   // 14
                "Electronic High"   // 15
                                              }};

        return distNames[static_cast<int>(dist)];
    }

}
