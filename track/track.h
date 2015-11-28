#ifndef TRACK_H
#define TRACK_H

#include <QObject>
#include "instrument.h"
#include <QList>

namespace Track {

    class Track
    {
    public:
        static const int numInstruments = 7;

        QString name{"New track"};

        QList<Instrument> instruments{
            {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"}
        };

        Track();

        int getNumUsedWaveformFrames();
        int getNumInstruments();
    };
}

#endif // TRACK_H
