#ifndef PATTERN_H
#define PATTERN_H

#include <QString>
#include <QList>
#include "note.h"

namespace Track {

class Pattern
{
public:
    Pattern();

    Pattern(QString name) : name(name) {}

    QString name;

    QList<Note> notes;
};

}

#endif // PATTERN_H
