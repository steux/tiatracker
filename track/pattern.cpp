#include "pattern.h"
#include <QJsonArray>


namespace Track {

Pattern::Pattern()
{

}

/*************************************************************************/

void Pattern::toJson(QJsonObject &json) {
    json["name"] = name;
    QJsonArray noteArray;
    for (int i = 0; i < notes.size(); ++i) {
        QJsonObject noteJson;
        notes[i].toJson(noteJson);
        noteArray.append(noteJson);
    }
    json["notes"] = noteArray;
}

/*************************************************************************/

bool Pattern::fromJson(const QJsonObject &json) {
    name = json["name"].toString();
    QJsonArray noteArray = json["notes"].toArray();
    notes.clear();
    for (int i = 0; i < noteArray.size(); ++i) {
        Note newNote;
        if (!newNote.fromJson(noteArray[i].toObject())) {
            return false;
        }
        notes.append(newNote);
    }
    return true;
}

}
