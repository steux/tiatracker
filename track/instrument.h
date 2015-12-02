#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <QString>
#include <QList>
#include "tiasound/tiasound.h"


namespace Track {

/* A TIATracker instrument, based on a base distortion and ADSR envelopes
 * for volume and frequency.
 */
class Instrument
{
public:
    Instrument(QString name) : name(name) {}

    bool isEmpty();

    int getEnvelopeLength();

    void setEnvelopeLength(int newSize);

    /* Checks if release starts after sustain and if not, changes
     * values accordingly. */
    void validateSustainReleaseValues();

    void setSustainAndRelease(int newSustainStart, int newReleaseStart);

    void deleteInstrument();

    /* Get minimum volume over the whole envelope */
    int getMinVolume();

    /* Get maximum volume over the whole envelope */
    int getMaxVolume();

    int getSustainStart() const;

    int getReleaseStart() const;

    QString name;
    TiaSound::Distortion baseDistortion{TiaSound::Distortion::BUZZY};
    QList<int> volumes{0, 0};
    QList<int> frequencies{0, 0};

private:
    int envelopeLength = 2;
    int sustainStart = 0;
    int releaseStart = 1;
};

}

#endif // INSTRUMENT_H
