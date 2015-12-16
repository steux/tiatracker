#ifndef TRACK_H
#define TRACK_H

#include <QObject>
#include "instrument.h"
#include <QList>
#include <QMutex>

namespace Track {

/* Represents a TIATracker track with instruments, percussion, patterns
 * and meta-data.
 */
class Track
{
public:
    static const int numInstruments = 7;

    Track();

    /* Lock or unlock track for thread-safe operations like modifying
     * envelope length of an instrument */
    void lock();
    void unlock();

    /* Counts all envelope frames over all instruments */
    int getNumUsedEnvelopeFrames();

    int getNumInstruments();

    QString name{"New track"};
    QList<Instrument> instruments{
        {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"}
    };

private:
    QMutex mutex;
};

}

#endif // TRACK_H
