#ifndef TRACK_H
#define TRACK_H

#include <QObject>
#include "instrument.h"
#include <QList>

namespace Track {

/* Represents a TIATracker track with instruments, percussion, patterns
 * and meta-data.
 */
class Track
{
public:
    static const int numInstruments = 7;

    Track();

    /* Counts all envelope frames over all instruments */
    int getNumUsedEnvelopeFrames();

    int getNumInstruments();

    QString name{"New track"};
    QList<Instrument> instruments{
        {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"}
    };
};

}

#endif // TRACK_H
