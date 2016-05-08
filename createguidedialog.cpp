#include "createguidedialog.h"
#include "ui_createguidedialog.h"

#include <QMap>
#include <QString>
#include "tiasound/tiasound.h"
#include "tiasound/pitchguidefactory.h"
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
    int threshold = ui->spinBoxGuideMaxOffTune->value();
    QString name = ui->lineEditGuideName->text();

    for (int i = 0; i < checkBoxNames.size(); ++i) {
        TiaSound::Distortion dist = checkBoxNames.keys()[i];
        QCheckBox *cbDist = findChild<QCheckBox *>(checkBoxNames[dist]);
    }
    TiaSound::TvStandard standard = ui->radioButtonGuidePal->isChecked() ? TiaSound::TvStandard::PAL : TiaSound::TvStandard::NTSC;
    TiaSound::PitchGuideFactory pg;

    // Optimize
    TiaSound::PitchGuide newGuide{name, standard};

    long bestError = 99999999;
    double bestFreq = 0;
    int bestNum = 0;
    //double freqDeviation =
/*
    // TODO
    for (double f = 330.0; f < 660.0; f += 0.1) {
        long currentError = 0;
        int curNum = 0;
        QList<FrequencyPitchGuide> guide = calcInstrumentPitchGuide(TvStandard::NTSC, Distortion::PURE_HIGH, f);
        for (int i = 0; i < guide.size(); ++i) {
            if (std::abs(guide[i].percentOff) <= threshold) {
                currentError += guide[i].percentOff*guide[i].percentOff;
                curNum++;
            }
        }
        if (curNum > bestNum || (curNum == bestNum && currentError < bestError)) {
            std::cout << "Found new best: " << f << ", " << curNum << ": " << currentError << "\n";
            bestError = currentError;
            bestFreq = f;
            bestNum = curNum;
        }
    }
*/
}
