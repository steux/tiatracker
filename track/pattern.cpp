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

}
