#ifndef PATTERN_H
#define PATTERN_H

#include <QString>
#include <QList>
#include "note.h"
#include <QJsonObject>

namespace Track {

class Pattern
{
public:
    Pattern();

    Pattern(QString name) : name(name) {}

    void toJson(QJsonObject &json);

    QString name;
    QList<Note> notes;
};

}

#endif // PATTERN_H
