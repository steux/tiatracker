#include "tracktab.h"
#include <QPushButton>
#include <QLayout>
#include <QButtonGroup>
#include <iostream>
#include "instrumentselector.h"
#include "timeline.h"
#include "patterneditor.h"
#include <QLabel>


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
    updateTrackStats();
}

/*************************************************************************/

void TrackTab::updateTrackStats() {
    // Patterns
    QLabel *statsLabel = findChild<QLabel *>("labelPatternsUsed");
    int sequenceLength = pTrack->channelSequences[0].sequence.size() + pTrack->channelSequences[1].sequence.size();
    int distinct = pTrack->patterns.size();
    statsLabel->setText("Patterns used: " + QString::number(sequenceLength) + " of 255 ("
                        + QString::number(distinct) + " distinct)");

    // Time
    int numRowsHalf = pTrack->getNumRows()/2;
    // FIXME
}

