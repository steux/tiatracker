#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <QString>

class Instrument
{
public:
    Instrument();

    QString name{};

    int waveformLength;
    int sustainStart;
    int releaseStart;

private:
};

#endif // INSTRUMENT_H
