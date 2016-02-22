#include "createpatterndialog.h"
#include "ui_createpatterndialog.h"
#include "track/track.h"
#include "track/pattern.h"
#include <iostream>


CreatePatternDialog::CreatePatternDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreatePatternDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    ui->spinBoxNewPatternLength->setMinimum(Track::Pattern::minSize);
    ui->spinBoxNewPatternLength->setMaximum(Track::Pattern::maxSize);
}

CreatePatternDialog::~CreatePatternDialog()
{
    delete ui;
}

/*************************************************************************/

void CreatePatternDialog::prepare(Track::Track *newTrack, int length, int selectedChannel, int selectedRow) {
    pTrack = newTrack;
    channel = selectedChannel;
    row = selectedRow;
    ui->spinBoxNewPatternLength->setValue(length);
    ui->lineEditNewPatternName->setText("New pattern");
}

/*************************************************************************/

QString CreatePatternDialog::getName() {
    return ui->lineEditNewPatternName->text();
}

/*************************************************************************/

int CreatePatternDialog::getLength() {
    return ui->spinBoxNewPatternLength->value();
}

/*************************************************************************/

void CreatePatternDialog::on_pushButtonAlign_clicked() {
    if (row >= pTrack->getChannelNumRows(1 - channel)) {
        return;
    }
    int otherEntryIndex = pTrack->getSequenceEntryIndex(1 - channel, row);
    int otherPatternIndex = pTrack->channelSequences[1 - channel].sequence[otherEntryIndex].patternIndex;
    int otherPatternSize = pTrack->patterns[otherPatternIndex].notes.size();
    int otherPatternFirstNote = pTrack->channelSequences[1 - channel].sequence[otherEntryIndex].firstNoteNumber;
    int newSize = otherPatternFirstNote + otherPatternSize - row;
    if (newSize < Track::Pattern::minSize) {
        newSize = Track::Pattern::minSize;
    }
    ui->spinBoxNewPatternLength->setValue(newSize);
}
