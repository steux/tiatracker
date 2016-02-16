#include "tracktab.h"
#include <QPushButton>
#include <QLayout>
#include <QButtonGroup>
#include <iostream>
#include "instrumentselector.h"
#include "timeline.h"
#include "patterneditor.h"
#include <QLabel>
#include <QSpinBox>


TrackTab::TrackTab(QWidget *parent) : QWidget(parent)
{
    patternContextMenu.addAction(&actionInsertPatternBefore);
    patternContextMenu.addAction(&actionInsertPatternAfter);
    patternContextMenu.addAction(&actionMovePatternUp);
    patternContextMenu.addAction(&actionMovePatternDown);
    patternContextMenu.addAction(&actionRemovePattern);
    patternContextMenu.addAction(&actionRenamePattern);
    patternContextMenu.addAction(&actionSetGoto);
    patternContextMenu.addAction(&actionRemoveGoto);
    patternContextMenu.addAction(&actionSetStartPattern);

    channelContextMenu.addAction(&actionPause);
    channelContextMenu.addAction(&actionHold);
    channelContextMenu.addAction(&actionSlide);
    channelContextMenu.addAction(&actionSetFrequency);
    channelContextMenu.addAction(&actionInsertRowBefore);
    channelContextMenu.addAction(&actionInsertRowAfter);
    channelContextMenu.addAction(&actionDeleteRow);
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
    editor->registerPatternMenu(&patternContextMenu);
    editor->registerChannelMenu(&channelContextMenu);
}

/*************************************************************************/

void TrackTab::updateTrackTab() {
    // Set GUI elements
    QSpinBox *spEven = findChild<QSpinBox *>("spinBoxEvenTempo");
    spEven->setValue(pTrack->evenSpeed);
    QSpinBox *spOdd = findChild<QSpinBox *>("spinBoxOddTempo");
    spOdd->setValue(pTrack->oddSpeed);
    QSpinBox *spRowsPerBeat = findChild<QSpinBox *>("spinBoxRowsPerBeat");
    spRowsPerBeat->setValue(pTrack->rowsPerBeat);

    // Update individual sub-widgets
    updateTrackStats();
}

/*************************************************************************/

void TrackTab::setEvenSpeed(int value) {
    pTrack->evenSpeed = value;
    updateTrackStats();
    updatePatternEditor();
}

/*************************************************************************/

void TrackTab::setOddSpeed(int value) {
    pTrack->oddSpeed = value;
    updateTrackStats();
    updatePatternEditor();
}

/*************************************************************************/

void TrackTab::updateTrackStats() {
    // Patterns
    QLabel *statsLabel = findChild<QLabel *>("labelPatternsUsed");
    int sequenceLength = pTrack->channelSequences[0].sequence.size() + pTrack->channelSequences[1].sequence.size();
    int distinct = pTrack->patterns.size();
    statsLabel->setText("Patterns used: " + QString::number(sequenceLength) + " of 255 ("
                        + QString::number(distinct) + " distinct)");

    // Start patterns
    QLabel *startLabel = findChild<QLabel *>("labelStartPatterns");
    startLabel->setText("Start patterns: " + QString::number(pTrack->startPatterns[0] + 1)
                        + "/" + QString::number(pTrack->startPatterns[1] + 1));

    // Time
    int numRows = pTrack->getTrackNumRows();
    long numOddTicks = int((numRows + 1)/2)*pTrack->oddSpeed;
    long numEvenTicks = int(numRows/2)*pTrack->evenSpeed;
    long numTicks = numOddTicks + numEvenTicks;
    int ticksPerSecond = pTrack->getTvMode() == TiaSound::TvStandard::PAL ? 50 : 60;
    int minutes = numTicks/(ticksPerSecond*60);
    int seconds = (numTicks%(ticksPerSecond*60))/ticksPerSecond;
    QString timeText = QString::number(minutes);
    if (seconds < 10) {
        timeText.append("m 0");
    } else {
        timeText.append("m ");
    }
    timeText.append(QString::number(seconds) + "s");
    QLabel *timeLabel = findChild<QLabel *>("labelTotalLength");
    timeLabel->setText("Total length: " + timeText);
}

/*************************************************************************/

void TrackTab::updatePatternEditor() {
    PatternEditor *editor = findChild<PatternEditor *>("trackEditor");
    editor->update();
}

