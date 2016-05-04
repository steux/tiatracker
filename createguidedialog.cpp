#include "createguidedialog.h"
#include "ui_createguidedialog.h"

#include <QMap>
#include <QString>
#include "tiasound/tiasound.h"
#include <iostream>


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
}

/*************************************************************************/

void CreateGuideDialog::on_pushButtonCreateGuide_clicked() {
    QMap<TiaSound::Distortion, QString> checkBoxNames{
        {TiaSound::Distortion::BUZZY, "checkBoxGuideBuzzy"},
        {TiaSound::Distortion::BUZZY_RUMBLE, "checkBoxGuideBuzzyRumble"},
        {TiaSound::Distortion::FLANGY_WAVERING, "checkBoxGuideFlangyWavering"},
        {TiaSound::Distortion::PURE_HIGH, "checkBoxGuidePureHigh"},
        {TiaSound::Distortion::PURE_BUZZY, "checkBoxGuidePureBuzzy"},
        {TiaSound::Distortion::REEDY_RUMBLE, "checkBoxGuideReedyRumble"},
        {TiaSound::Distortion::WHITE_NOISE, "checkBoxGuideWhiteNoise"},
        {TiaSound::Distortion::PURE_LOW, "checkBoxGuidePureLow"},
        {TiaSound::Distortion::ELECTRONIC_RUMBLE, "checkBoxGuideElectronicRumble"},
        {TiaSound::Distortion::ELECTRONIC_SQUEAL, "checkBoxGuideElectronicSqueal"}
    };

    int maxDeviation = ui->spinBoxGuideMaxDeviation->value();
    int maxOffTune = ui->spinBoxGuideMaxOffTune->value();

    for (int i = 0; i < checkBoxNames.size(); ++i) {
        TiaSound::Distortion dist = checkBoxNames.keys()[i];
        QCheckBox *cbDist = findChild<QCheckBox *>(checkBoxNames[dist]);
    }
}
