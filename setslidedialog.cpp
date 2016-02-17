#include "setslidedialog.h"
#include "ui_setslidedialog.h"

SetSlideDialog::SetSlideDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetSlideDialog)
{
    ui->setupUi(this);
}

SetSlideDialog::~SetSlideDialog()
{
    delete ui;
}
