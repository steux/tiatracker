/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

#include "sequence.h"
#include <QJsonArray>
#include "mainwindow.h"


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
            MainWindow::displayMessage("Invalid sequence entry at " + QString::number(i));
            return false;
        }
        if (se.gotoTarget < -1 || se.gotoTarget >= seqArray.size()
                || se.patternIndex < 0) {
            MainWindow::displayMessage("Invalid goto target at " + QString::number(i));
            return false;
        }
        sequence.append(se);
    }
    return true;
}

}
