#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <QList>
#include "pattern.h"
#include "sequenceentry.h"
#include <QJsonObject>


namespace Track {

class Sequence
{
public:
    Sequence();

    void toJson(QJsonObject &json);

    QList<SequenceEntry> sequence{};
};

}

#endif // SEQUENCE_H
