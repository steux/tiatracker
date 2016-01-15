#ifndef NOTE_H
#define NOTE_H


class Note
{
public:
    enum class instrumentType {Instrument, Percussion, Hold, Pause, Slide};

    Note();

    instrumentType type;
    // 0-6 for instrument, 0-14 for percussion
    int instrumendNumber;
    // Depending on type: frequency, or slide value
    int value;
};

#endif // NOTE_H
