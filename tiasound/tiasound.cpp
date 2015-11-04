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

}
