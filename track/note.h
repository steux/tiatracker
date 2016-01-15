#ifndef NOTE_H
#define NOTE_H

namespace Track {

class Note
{
public:
    enum class instrumentType {Instrument, Percussion, Hold, Pause, Slide};

    Note();
    Note(instrumentType type, int instrumentNumber, int value)
        : type(type), instrumentNumber(instrumentNumber), value(value) {}

    instrumentType type;
    // 0-6 for instrument, 0-14 for percussion
    int instrumentNumber;
    // Depending on type: frequency, or slide value
    int value;
};

}

#endif // NOTE_H
