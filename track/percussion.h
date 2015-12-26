#ifndef PERCUSSION_H
#define PERCUSSION_H

#include <QString>
#include <QList>
#include <QMutex>
#include "tiasound/tiasound.h"
#include <QJsonObject>


namespace Track {

class Percussion
{
public:
    Percussion(QString name) : name(name) {}

    int getEnvelopeLength();
    void setEnvelopeLength(int newSize);

    // Checks if instrument has its empty starting values
    bool isEmpty();

    void toJson(QJsonObject &json);
    bool import(const QJsonObject &json);

    void deletePercussion();

    /* Get minimum volume over the whole envelope */
    int getMinVolume();

    /* Get maximum volume over the whole envelope */
    int getMaxVolume();

    QString name;
    QList<int> volumes{0};
    QList<int> frequencies{0};
    QList<TiaSound::Distortion> waveforms{TiaSound::Distortion::WHITE_NOISE};

private:
    int envelopeLength = 1;


};

}

#endif // PERCUSSION_H
