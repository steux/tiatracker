#include "createpatterndialog.h"
#include "ui_createpatterndialog.h"

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

void CreatePatternDialog::prepare(Track::Track *newTrack, int length) {
    pTrack = newTrack;
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


