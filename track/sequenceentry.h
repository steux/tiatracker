#ifndef SEQUENCEENTRY_H
#define SEQUENCEENTRY_H

namespace Track {

class SequenceEntry
{
public:
    SequenceEntry();
    SequenceEntry(int patternIndex, int gotoTarget = -1) :
        patternIndex(patternIndex), gotoTarget(gotoTarget) {}

    int patternIndex;
    int gotoTarget = -1;

    // The note number from the track of the first note of this pattern
    int firstNoteNumber;
};

}

#endif // SEQUENCEENTRY_H
