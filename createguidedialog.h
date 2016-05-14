#ifndef CREATEGUIDEDIALOG_H
#define CREATEGUIDEDIALOG_H

#include <QDialog>
#include "tiasound/tiasound.h"
#include "tiasound/pitchguide.h"
#include <QMap>


namespace Ui {
class CreateGuideDialog;
}

class CreateGuideDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateGuideDialog(QWidget *parent = 0);
    ~CreateGuideDialog();

protected:
    // Prevent enter presses in QLineEdit to trigger OK button
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;

private slots:
    void on_pushButtonCreateGuide_clicked();

    void on_comboBoxGuideWaveforms_currentIndexChanged(int index);

private:
    const QMap<TiaSound::Distortion, QString> checkBoxNames{
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

    TiaSound::PitchGuide newGuide{"", TiaSound::TvStandard::PAL, 0.0};
    bool isGuideCreated = false;

    Ui::CreateGuideDialog *ui;
};

#endif // CREATEGUIDEDIALOG_H
