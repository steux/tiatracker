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
    const static int minSize = 1;
    const static int maxSize = 127;

    Pattern();

    Pattern(QString name) : name(name) {}

    void toJson(QJsonObject &json);
    bool fromJson(const QJsonObject &json);

    QString name;
    QList<Note> notes;
};

}

#endif // PATTERN_H
