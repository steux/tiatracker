#include "tracktab.h"
#include <QPushButton>
#include <QLayout>
#include <QButtonGroup>
#include <iostream>
#include "instrumentselector.h"
#include "timeline.h"
#include "patterneditor.h"


TrackTab::TrackTab(QWidget *parent) : QWidget(parent)
{

}

/*************************************************************************/

void TrackTab::registerTrack(Track::Track *newTrack) {
    pTrack = newTrack;
}

/*************************************************************************/

void TrackTab::registerPitchGuide(TiaSound::PitchGuide *newGuide) {
    pPitchGuide = newGuide;
}

/*************************************************************************/

void TrackTab::initTrackTab() {
    pTrack->updateFirstNoteNumbers();

    InstrumentSelector *insSel = findChild<InstrumentSelector *>("trackInstrumentSelector");
    insSel->registerTrack(pTrack);
    Timeline *timeline = findChild<Timeline *>("trackTimeline");
    timeline->registerTrack(pTrack);
    PatternEditor *editor = findChild<PatternEditor *>("trackEditor");
    editor->registerTrack(pTrack);
    editor->registerPitchGuide(pPitchGuide);

}

/*************************************************************************/

void TrackTab::updateTrackTab() {

}

