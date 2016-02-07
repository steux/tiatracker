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

}
