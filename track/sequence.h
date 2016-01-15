#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <QList>
#include "pattern.h"
#include "sequenceentry.h"


namespace Track {

class Sequence
{
public:
    Sequence();

    QList<SequenceEntry> sequence{};
};

}

#endif // SEQUENCE_H
