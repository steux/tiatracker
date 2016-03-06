#ifndef TRACK_H
#define TRACK_H

#include <QObject>
#include "instrument.h"
#include "percussion.h"
#include <QList>
#include <QMutex>
#include "tiasound/tiasound.h"
#include "pattern.h"
#include "sequence.h"
#include <QJsonObject>


namespace Track {

/* Represents a TIATracker track with instruments, percussion, patterns
 * and meta-data.
 */
class Track
{
public:
    static const int numInstruments = 7;
    static const int numPercussion = 15;

    Track();

    /* Lock or unlock track for thread-safe operations like modifying
     * envelope length of an instrument */
    void lock();
    void unlock();

    void newTrack();

    /* Counts all envelope frames over all instruments */
    int getNumUsedEnvelopeFrames();
    int getNumUsedPercussionFrames();

    int getNumUsedInstruments();
    int getNumUsedPercussion();

    /* Get number of rows of one channel */
    int getChannelNumRows(int channel);

    /* Get max number of rows of both channels */
    int getTrackNumRows();

    /* Sets "firstNoteNumber" values of all sequence entries */
    void updateFirstNoteNumbers();

    /* For a given channel, return the index of the SequenceEntry
     * a given (valid) row is in. */
    int getSequenceEntryIndex(int channel, int row);

    int getPatternIndex(int channel, int row);

    int getNoteIndexInPattern(int channel, int row);

    /* Get index of next note (and entry, if new pattern is reached)
     * for a specific channel.
     * Returns false if there is no next note. */
    bool getNextNote(int channel, int *pEntryIndex, int *pPatternNoteIndex);

    /* Get index of next note (and entry, if new pattern is reached)
     * for a specific channel, observing goto.
     * Returns false if there is no next note. */
    bool getNextNoteWithGoto(int channel, int *pEntryIndex, int *pPatternNoteIndex);

    bool checkSlideValidity(int channel, int row);

    int skipInstrumentType(int channel, int row, Note::instrumentType type, int direction);

    Note *getNote(int channel, int row);

    /* Goes sequentially through a pattern and finds and corrects
     * invalid notes, like PAUSE after a percussion, etc.
     * Does not consider notes outside the pattern. */
    void validatePattern(int patternIndex);

    void toJson(QJsonObject &json);
    bool fromJson(const QJsonObject &json);

    TiaSound::TvStandard getTvMode() const;
    void setTvMode(const TiaSound::TvStandard &value);

    QString name{"new track.ttt"};
    QList<Instrument> instruments{
        {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"}
    };
    QList<Percussion> percussion{
        {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"},
        {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"}
    };

    QList<Pattern> patterns{};
    QList<Sequence> channelSequences{{}, {}};
    int evenSpeed = 5;
    int oddSpeed = 5;
    int rowsPerBeat = 4;
    int startPatterns[2]{};
    int playPos[2]{};

private:
    QMutex mutex;

    TiaSound::TvStandard tvMode = TiaSound::TvStandard::PAL;
};

}

#endif // TRACK_H
