#ifndef TRACK_H
#define TRACK_H

#include <QObject>
#include "instrument.h"
#include <QList>

namespace Track {

    class Track
    {
    public:
        QString name{"New track"};

        QList<Instrument> instruments{
            {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"}
        };

        Track();

        int getUsedWaveformFrames();
    };
}

#endif // TRACK_H
