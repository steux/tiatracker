#include "optionstab.h"
#include <QComboBox>
#include <QSpinBox>
#include "createguidedialog.h"


OptionsTab::OptionsTab(QWidget *parent) : QWidget(parent)
{
    TiaSound::PitchGuideFactory pgFactory;
    guides.append(pgFactory.getPitchPerfectPalGuide());
    guides.append(pgFactory.getOptimizedPurePalGuide());
    guides.append(pgFactory.getOptimizedDiv31PalGuide());
    guides.append(pgFactory.getOptimizedDiv6PalGuide());
    guides.append(pgFactory.getOptimizedDiv2PalGuide());
    guides.append(pgFactory.getPitchPerfectNtscGuide());
    guides.append(pgFactory.getOptimizedPureNtscGuide());
    guides.append(pgFactory.getOptimizedDiv31NtscGuide());
    guides.append(pgFactory.getOptimizedDiv6NtscGuide());
    guides.append(pgFactory.getOptimizedDiv2NtscGuide());
}

/*************************************************************************/

void OptionsTab::registerTrack(Track::Track *newTrack) {
    pTrack = newTrack;
}

/*************************************************************************/

void OptionsTab::initOptionsTab() {
    QComboBox *cbGuides = findChild<QComboBox *>("comboBoxPitchGuide");
    for (int i = 0; i < guides.size(); ++i) {
        cbGuides->addItem(guides[i].name);
    }
}

/*************************************************************************/

void OptionsTab::updateOptionsTab() {

}

/*************************************************************************/

void OptionsTab::on_comboBoxPitchGuide_currentIndexChanged(int index) {
    emit setPitchGuide(guides[index]);
}

/*************************************************************************/

void OptionsTab::on_radioButtonPal_toggled(bool checked) {
    if (checked) {
        // Set to PAL
        pTrack->setTvMode(TiaSound::TvStandard::PAL);
        emit setTVStandard(TiaSound::TvStandard::PAL);
    } else {
        // Set to NTSC
        pTrack->setTvMode(TiaSound::TvStandard::NTSC);
        emit setTVStandard(TiaSound::TvStandard::NTSC);
    }
}

/*************************************************************************/

void OptionsTab::on_spinBoxOffTuneThreshold_editingFinished() {
    QSpinBox *sbThreshold = findChild<QSpinBox *>("spinBoxOffTuneThreshold");
    emit setOffTuneThreshold(sbThreshold->value());
}

/*************************************************************************/

void OptionsTab::on_pushButtonGuideCreate_clicked(bool) {
    CreateGuideDialog newDialog(this);
    if (newDialog.exec() == QDialog::Accepted) {

    }
}

