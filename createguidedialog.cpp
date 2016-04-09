#include "createguidedialog.h"
#include "ui_createguidedialog.h"

CreateGuideDialog::CreateGuideDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateGuideDialog)
{
    ui->setupUi(this);
}

CreateGuideDialog::~CreateGuideDialog()
{
    delete ui;
}
