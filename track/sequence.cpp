#include "sequence.h"
#include <QJsonArray>


namespace Track {

Sequence::Sequence()
{

}

/*************************************************************************/

void Sequence::toJson(QJsonObject &json) {
    QJsonArray seqArray;
    for (int i = 0; i < sequence.size(); ++i) {
        QJsonObject seqJson;
        sequence[i].toJson(seqJson);
        seqArray.append(seqJson);
    }
    json["sequence"] = seqArray;
}

/*************************************************************************/

bool Sequence::fromJson(const QJsonObject &json) {
    QJsonArray seqArray = json["sequence"].toArray();
    sequence.clear();
    for (int i = 0; i < seqArray.size(); ++i) {
        SequenceEntry se;
        if (!se.fromJson(seqArray[i].toObject())) {
            return false;
        }
        if (se.gotoTarget < -1 || se.gotoTarget >= seqArray.size()
                || se.patternIndex < 0) {
            return false;
        }
        sequence.append(se);
    }
    return true;
}

}
