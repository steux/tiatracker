#include "setgotodialog.h"
#include "ui_setgotodialog.h"

SetGotoDialog::SetGotoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetGotoDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
}

SetGotoDialog::~SetGotoDialog()
{
    delete ui;
}

/*************************************************************************/

void SetGotoDialog::setMaxValue(int max) {
    ui->spinBoxSetGoto->setMaximum(max);
}

/*************************************************************************/

void SetGotoDialog::setGotoValue(int value) {
    ui->spinBoxSetGoto->setValue(value);
}

/*************************************************************************/

int SetGotoDialog::getGotoValue() {
    return ui->spinBoxSetGoto->value();
}
