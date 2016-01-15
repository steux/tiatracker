#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <QList>
#include "pattern.h"


class Sequence
{
public:
    Sequence();

    QList<Pattern> patterns{};
};

#endif // SEQUENCE_H
