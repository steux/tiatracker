#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <QString>
#include <QList>
#include <QMutex>
#include "tiasound/tiasound.h"
#include <QJsonObject>


namespace Track {

/* A TIATracker instrument, based on a base distortion and ADSR envelopes
 * for volume and frequency.
 */
class Instrument
{
public:
    Instrument(QString name) : name(name) {}

    // To make read/write access to envelope length thread-safe, call lock/unlock
    void lock();
    void unlock();

    int getEnvelopeLength();
    void setEnvelopeLength(int newSize);

    // Return AUDC value. Can be different per frequency for PURE COMBINED.
    int getAudCValue(int frequency);

    // Checks if instrument has its empty starting values
    bool isEmpty();

    void toJson(QJsonObject &json);

    void deleteInstrument();

    /* Checks if release starts after sustain and if not, changes
     * values accordingly. */
    void validateSustainReleaseValues();
    void setSustainAndRelease(int newSustainStart, int newReleaseStart);

    /* Get minimum volume over the whole envelope */
    int getMinVolume();

    /* Get maximum volume over the whole envelope */
    int getMaxVolume();

    int getSustainStart() const;
    int getReleaseStart() const;

    QString name;
    TiaSound::Distortion baseDistortion{TiaSound::Distortion::PURE_COMBINED};
    QList<int> volumes{0, 0};
    QList<int> frequencies{0, 0};

private:
    int envelopeLength = 2;
    int sustainStart = 0;
    int releaseStart = 1;
};

}

#endif // INSTRUMENT_H
