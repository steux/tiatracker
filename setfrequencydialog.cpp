/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

#include "setfrequencydialog.h"
#include "ui_setfrequencydialog.h"

SetFrequencyDialog::SetFrequencyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetFrequencyDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
}

SetFrequencyDialog::~SetFrequencyDialog()
{
    delete ui;
}

/*************************************************************************/

void SetFrequencyDialog::setMaxFrequencyValue(int max) {
    ui->spinBoxSetFrequency->setMaximum(max);
}

/*************************************************************************/

void SetFrequencyDialog::setFrequencyValue(int value) {
    ui->spinBoxSetFrequency->setValue(value);
}

/*************************************************************************/

int SetFrequencyDialog::getFrequencyValue() {
    return ui->spinBoxSetFrequency->value();
}
