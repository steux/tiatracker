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

}
