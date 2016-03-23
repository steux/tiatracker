#include "optionstab.h"
#include <QComboBox>


OptionsTab::OptionsTab(QWidget *parent) : QWidget(parent)
{
    TiaSound::PitchGuideFactory pgFactory;
    guides.append(pgFactory.getPitchPerfectPalGuide());
    guides.append(pgFactory.getPitchPerfectNtscGuide());
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

