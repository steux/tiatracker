#include "createguidedialog.h"
#include "ui_createguidedialog.h"

#include <QMap>
#include <QString>
#include "tiasound/tiasound.h"


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

/*************************************************************************/

void CreateGuideDialog::on_pushButton_clicked() {
    QMap<TiaSound::Distortion, QString> checkBoxNames{
        {TiaSound::Distortion::BUZZY, "checkBoxGuideBuzzy"},
        {TiaSound::Distortion::},
        {TiaSound::Distortion::},
        {TiaSound::Distortion::},
        {TiaSound::Distortion::},
        {TiaSound::Distortion::},
        {TiaSound::Distortion::},
        {TiaSound::Distortion::},
        {TiaSound::Distortion::},
        {TiaSound::Distortion::},
        {TiaSound::Distortion::}
    };

    int maxDeviation = ui->spinBoxGuideMaxDeviation->value();
    int maxOffTune = ui->spinBoxGuideMaxOffTune->value();

}
