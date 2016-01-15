#ifndef PATTERN_H
#define PATTERN_H

#include <QString>
#include <QList>
#include "note.h"


class Pattern
{
public:
    Pattern();

    QString name;

    // Target of a following goto, or -1 of no goto
    int gotoTarget = -1;

    QList<Note> notes;
};

#endif // PATTERN_H
