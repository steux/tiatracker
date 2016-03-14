/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

#include "setslidedialog.h"
#include "ui_setslidedialog.h"

SetSlideDialog::SetSlideDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetSlideDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
}

SetSlideDialog::~SetSlideDialog()
{
    delete ui;
}

/*************************************************************************/

int SetSlideDialog::getSlideValue() {
    return ui->spinBoxSetSlideValue->value();
}

/*************************************************************************/

void SetSlideDialog::setSlideValue(int value) {
    ui->spinBoxSetSlideValue->setValue(value);
}

