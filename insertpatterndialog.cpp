/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

#include "insertpatterndialog.h"
#include "ui_insertpatterndialog.h"

InsertPatternDialog::InsertPatternDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertPatternDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
}

InsertPatternDialog::~InsertPatternDialog()
{
    delete ui;
}

/*************************************************************************/

void InsertPatternDialog::prepare(Track::Track *newTrack) {
    pTrack = newTrack;
    ui->comboBoxInsertPattern->clear();
    // Fill combo box with pattern names and stats
    for (int i = 0; i < pTrack->patterns.size(); ++i) {
        QString patternName = pTrack->patterns[i].name;
        int count = 0;
        for (int channel = 0; channel < 2; ++channel) {
            for (int j = 0; j < pTrack->channelSequences[channel].sequence.size(); ++j) {
                if (pTrack->channelSequences[channel].sequence[j].patternIndex == i) {
                    count++;
                }
            }
        }
        patternName.append(" (len " + QString::number(pTrack->patterns[i].notes.size()) + ", "
                           + QString::number(count) + "x used)");
        ui->comboBoxInsertPattern->addItem(patternName);
    }
    ui->comboBoxInsertPattern->addItem("Create new pattern");
}

/*************************************************************************/

int InsertPatternDialog::getSelectedPattern() {
    return ui->comboBoxInsertPattern->currentIndex();
}

/*************************************************************************/

void InsertPatternDialog::on_pushButtonCreateNewPattern_clicked() {
    ui->comboBoxInsertPattern->setCurrentIndex(ui->comboBoxInsertPattern->count() - 1);
    accept();
}
