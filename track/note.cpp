#include "note.h"
#include <stdexcept>

namespace Track {

const QList<Note::instrumentType> Note::insTypes{
    Note::instrumentType::Hold,
    Note::instrumentType::Instrument,
    Note::instrumentType::Pause,
    Note::instrumentType::Percussion,
    Note::instrumentType::Slide
};

Note::Note()
{

}

/*************************************************************************/

void Note::toJson(QJsonObject &json) {
    json["type"] = insTypes.indexOf(type);
    json["number"] = instrumentNumber;
    json["value"] = value;
}

}

