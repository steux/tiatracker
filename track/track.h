#ifndef TRACK_H
#define TRACK_H

#include <QObject>
#include "instrument.h"
#include <QList>

namespace Track {
    class Track
    {
    public:
        QList<Instrument> instruments{
            {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"}
        };

        Track();
    };
}

#endif // TRACK_H
