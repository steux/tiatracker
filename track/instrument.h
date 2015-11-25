#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <QString>
#include <QList>
#include "tiasound/tiasound.h"


namespace Track {

    class Instrument
    {
    public:
        Instrument(QString name) : name(name) {}

        QString name;
        TiaSound::Distortion baseDistortion;
        QList<int> volumes{0, 0};
        QList<int> frequencies{0, 0};

        int getWaveformLength() { return volumes.size(); }
        void setSustainAndRelease(int newSustainStart, int newReleaseStart);

    private:
        int sustainStart = 0;
        int releaseStart = 1;
    };

}

#endif // INSTRUMENT_H
