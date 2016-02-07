#include "sequenceentry.h"

namespace Track {

SequenceEntry::SequenceEntry()
{

}

/*************************************************************************/

void SequenceEntry::toJson(QJsonObject &json) {
    json["patternindex"] = patternIndex;
    json["gototarget"] = gotoTarget;
}

/*************************************************************************/

bool SequenceEntry::fromJson(const QJsonObject &json) {
    patternIndex = json["patternindex"].toInt();
    gotoTarget = json["gototarget"].toInt();
    if (patternIndex < 0 || gotoTarget < -1) {
        return false;
    }
    return true;
}

}
