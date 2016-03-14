/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

#include "renamepatterndialog.h"
#include "ui_renamepatterndialog.h"

RenamePatternDialog::RenamePatternDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenamePatternDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
}

RenamePatternDialog::~RenamePatternDialog()
{
    delete ui;
}

/*************************************************************************/

void RenamePatternDialog::setPatternName(QString &name) {
    ui->lineEditRenamePattern->setText(name);
}

/*************************************************************************/

QString RenamePatternDialog::getPatternName() {
    return ui->lineEditRenamePattern->text();
}
