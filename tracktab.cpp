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
#include "setslidedialog.h"
#include <QMessageBox>
#include "setfrequencydialog.h"
#include "instrumentstab.h"
#include "renamepatterndialog.h"
#include "setgotodialog.h"
#include "mainwindow.h"


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
    actionMuteChannel.setCheckable(true);
    channelContextMenu.addAction(&actionMuteChannel);
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

    // Pattern context menu
    QObject::connect(&actionSetStartPattern, SIGNAL(triggered(bool)), this, SLOT(setStartPattern(bool)));
    QObject::connect(&actionRenamePattern, SIGNAL(triggered(bool)), this, SLOT(renamePattern(bool)));
    QObject::connect(&actionSetGoto, SIGNAL(triggered(bool)), this, SLOT(setGoto(bool)));
    QObject::connect(&actionRemoveGoto, SIGNAL(triggered(bool)), this, SLOT(removeGoto(bool)));
    QObject::connect(&actionMovePatternUp, SIGNAL(triggered(bool)), this, SLOT(movePatternUp(bool)));
    QObject::connect(&actionMovePatternDown, SIGNAL(triggered(bool)), this, SLOT(movePatternDown(bool)));

    // Channel context menu
    QObject::connect(&actionSlide, SIGNAL(triggered(bool)), this, SLOT(setSlideValue(bool)));
    QObject::connect(&actionSetFrequency, SIGNAL(triggered(bool)), this, SLOT(setFrequency(bool)));
    QObject::connect(&actionHold, SIGNAL(triggered(bool)), this, SLOT(setHold(bool)));
    QObject::connect(&actionPause, SIGNAL(triggered(bool)), this, SLOT(setPause(bool)));
    QObject::connect(&actionDeleteRow, SIGNAL(triggered(bool)), this, SLOT(deleteRow(bool)));

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

void TrackTab::channelContextEvent(int channel, int noteIndex) {
    contextEventChannel = channel;
    contextEventNoteIndex = noteIndex;
}

/*************************************************************************/

void TrackTab::setStartPattern(bool) {
    pTrack->startPatterns[contextEventChannel] = pTrack->getSequenceEntryIndex(contextEventChannel, contextEventNoteIndex);
    update();
}

/*************************************************************************/

void TrackTab::renamePattern(bool) {
    RenamePatternDialog dialog(this);
    int entryIndex = pTrack->getSequenceEntryIndex(contextEventChannel, contextEventNoteIndex);
    int patternIndex = pTrack->channelSequences[contextEventChannel].sequence[entryIndex].patternIndex;
    dialog.setPatternName(pTrack->patterns[patternIndex].name);
    if (dialog.exec() == QDialog::Accepted) {
        pTrack->patterns[patternIndex].name = dialog.getPatternName();
        update();
    }
}

/*************************************************************************/

void TrackTab::setGoto(bool) {
    int maxValue = pTrack->channelSequences[contextEventChannel].sequence.size();
    SetGotoDialog dialog(this);
    dialog.setMaxValue(maxValue);
    int entryIndex = pTrack->getSequenceEntryIndex(contextEventChannel, contextEventNoteIndex);
    Track::SequenceEntry *entry = &(pTrack->channelSequences[contextEventChannel].sequence[entryIndex]);
    dialog.setGotoValue(std::max(1, entry->gotoTarget + 1));
    if (dialog.exec() == QDialog::Accepted) {
        entry->gotoTarget = dialog.getGotoValue() - 1;
        update();
    }

}

/*************************************************************************/

void TrackTab::removeGoto(bool) {
    int entryIndex = pTrack->getSequenceEntryIndex(contextEventChannel, contextEventNoteIndex);
    Track::SequenceEntry *entry = &(pTrack->channelSequences[contextEventChannel].sequence[entryIndex]);
    entry->gotoTarget = -1;
    update();
}

/*************************************************************************/

void TrackTab::movePatternUp(bool) {
    int entryIndex = pTrack->getSequenceEntryIndex(contextEventChannel, contextEventNoteIndex);
    if (entryIndex > 0) {
        pTrack->channelSequences[contextEventChannel].sequence.swap(entryIndex, entryIndex - 1);
        pTrack->updateFirstNoteNumbers();
        update();
    }
}

/*************************************************************************/

void TrackTab::movePatternDown(bool) {
    int entryIndex = pTrack->getSequenceEntryIndex(contextEventChannel, contextEventNoteIndex);
    if (entryIndex != pTrack->channelSequences[contextEventChannel].sequence.size() - 1) {
        pTrack->channelSequences[contextEventChannel].sequence.swap(entryIndex, entryIndex + 1);
        pTrack->updateFirstNoteNumbers();
        update();
    }
}

/*************************************************************************/

void TrackTab::setSlideValue(bool) {
    if (!pTrack->checkSlideValidity(contextEventChannel, contextEventNoteIndex)) {
        MainWindow::displayMessage("A SLIDE can only follow a melodic instrument or another slide!");
        return;
    }

    SetSlideDialog dialog(this);
    // If selected row already contains a SLIDE, pre-set dialog to that value
    Track::Note *selectedNote = pTrack->getNote(contextEventChannel, contextEventNoteIndex);
    if (selectedNote->type == Track::Note::instrumentType::Slide) {
        dialog.setSlideValue(selectedNote->value);
    }
    if (dialog.exec() == QDialog::Accepted) {
        selectedNote->type = Track::Note::instrumentType::Slide;
        selectedNote->value = dialog.getSlideValue();
        updatePatternEditor();
    }
}

/*************************************************************************/

void TrackTab::setFrequency(bool) {
    Track::Note *selectedNote = pTrack->getNote(contextEventChannel, contextEventNoteIndex);
    if (selectedNote->type != Track::Note::instrumentType::Instrument) {
        MainWindow::displayMessage("Only a melodic instrument can have a frequency value!");
        return;
    }
    SetFrequencyDialog dialog(this);
    dialog.setFrequencyValue(selectedNote->value);
    Track::Instrument *ins = &(pTrack->instruments[selectedNote->instrumentNumber]);
    int maxFreq = ins->baseDistortion == TiaSound::Distortion::PURE_COMBINED ? 63 : 31;
    dialog.setMaxFrequencyValue(maxFreq);
    if (dialog.exec() == QDialog::Accepted) {
        selectedNote->value = dialog.getFrequencyValue();
        updatePatternEditor();
    }
}

/*************************************************************************/

void TrackTab::setHold(bool) {
    pTrack->getNote(contextEventChannel, contextEventNoteIndex)->type = Track::Note::instrumentType::Hold;
    update();
}

/*************************************************************************/

void TrackTab::setPause(bool) {
    pTrack->getNote(contextEventChannel, contextEventNoteIndex)->type = Track::Note::instrumentType::Pause;
    int patternIndex = pTrack->getPatternIndex(contextEventChannel, contextEventNoteIndex);
    pTrack->validatePattern(patternIndex);
    update();

}

/*************************************************************************/

void TrackTab::deleteRow(bool) {
    int patternIndex = pTrack->getPatternIndex(contextEventChannel, contextEventNoteIndex);
    Track::Pattern *pattern = &(pTrack->patterns[patternIndex]);
    if (pattern->notes.size() == Track::Pattern::minSize) {
        MainWindow::displayMessage("Pattern is already at minimum size!");
        return;
    }

    int noteInPattern = pTrack->getNoteIndexInPattern(contextEventChannel, contextEventNoteIndex);
    pattern->notes.removeAt(noteInPattern);
    pTrack->updateFirstNoteNumbers();
    pTrack->validatePattern(patternIndex);
    update();
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

