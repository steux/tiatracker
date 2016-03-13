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
#include "insertpatterndialog.h"
#include "createpatterndialog.h"
#include <qcheckbox.h>


TrackTab::TrackTab(QWidget *parent) : QWidget(parent)
{
    // Global
    addShortcut(&actionMoveUp, "CursorUp");
    addShortcut(&actionMoveDown, "CursorDown");
    addShortcut(&actionLeftChannel, "CursorLeftChannel");
    addShortcut(&actionRightChannel, "CursorRightChannel");
    addShortcut(&actionSwitchChannel, "CursorSwitchChannel");
    addShortcut(&actionFirstRow, "CursorFirstRow");
    addShortcut(&actionLastRow, "CursorLastRow");
    addShortcut(&actionNextPattern, "CursorNextPattern");
    addShortcut(&actionPreviousPattern, "CursorPreviousPattern");

    // Pattern
    addShortcut(&actionInsertPatternBefore, "PatternInsert");
    patternContextMenu.addAction(&actionInsertPatternBefore);
    patternContextMenu.addAction(&actionInsertPatternAfter);
    addShortcut(&actionMovePatternUp, "PatternMoveUp");
    patternContextMenu.addAction(&actionMovePatternUp);
    addShortcut(&actionMovePatternDown, "PatternMoveDown");
    patternContextMenu.addAction(&actionMovePatternDown);
    addShortcut(&actionDuplicatePattern, "PatternDuplicate");
    patternContextMenu.addAction(&actionDuplicatePattern);
    addShortcut(&actionRemovePattern, "PatternRemove");
    patternContextMenu.addAction(&actionRemovePattern);
    addShortcut(&actionRenamePattern, "PatternRename");
    patternContextMenu.addAction(&actionRenamePattern);
    patternContextMenu.addSeparator();
    addShortcut(&actionSetGoto, "PatternSetGoto");
    patternContextMenu.addAction(&actionSetGoto);
    addShortcut(&actionRemoveGoto, "PatternRemoveGoto");
    patternContextMenu.addAction(&actionRemoveGoto);
    patternContextMenu.addAction(&actionSetStartPattern);
    // Channel
    addShortcut(&actionPause, "NotePause");
    channelContextMenu.addAction(&actionPause);
    addShortcut(&actionHold, "NoteHold");
    channelContextMenu.addAction(&actionHold);
    addShortcut(&actionSlide, "NoteSlide");
    channelContextMenu.addAction(&actionSlide);
    addShortcut(&actionSetFrequency, "NoteFrequency");
    channelContextMenu.addAction(&actionSetFrequency);
    channelContextMenu.addSeparator();
    addShortcut(&actionInsertRowBefore, "RowInsert");
    channelContextMenu.addAction(&actionInsertRowBefore);
    channelContextMenu.addAction(&actionInsertRowAfter);
    addShortcut(&actionDeleteRow, "RowDelete");
    channelContextMenu.addAction(&actionDeleteRow);
    channelContextMenu.addSeparator();
    actionMuteChannel.setCheckable(true);
    addShortcut(&actionMuteChannel, "ChannelToggleMute");
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
    insSel->initSelector();
    Timeline *timeline = findChild<Timeline *>("trackTimeline");
    timeline->registerTrack(pTrack);
    PatternEditor *editor = findChild<PatternEditor *>("trackEditor");
    editor->registerTrack(pTrack);
    editor->registerPitchGuide(pPitchGuide);
    editor->registerInstrumentSelector(insSel);

    // Gloabl actions
    QObject::connect(&actionMoveUp, SIGNAL(triggered(bool)), editor, SLOT(moveUp(bool)));
    QObject::connect(&actionMoveDown, SIGNAL(triggered(bool)), editor, SLOT(moveDown(bool)));
    QObject::connect(&actionLeftChannel, SIGNAL(triggered(bool)), editor, SLOT(moveLeft(bool)));
    QObject::connect(&actionRightChannel, SIGNAL(triggered(bool)), editor, SLOT(moveRight(bool)));
    QObject::connect(&actionSwitchChannel, SIGNAL(triggered(bool)), editor, SLOT(switchChannel(bool)));
    QObject::connect(&actionFirstRow, SIGNAL(triggered(bool)), editor, SLOT(gotoFirstRow(bool)));
    QObject::connect(&actionLastRow, SIGNAL(triggered(bool)), editor, SLOT(gotoLastRow(bool)));
    QObject::connect(&actionNextPattern, SIGNAL(triggered(bool)), editor, SLOT(gotoNextPattern(bool)));
    QObject::connect(&actionPreviousPattern, SIGNAL(triggered(bool)), editor, SLOT(gotoPreviousPattern(bool)));

    // Pattern context menu
    QObject::connect(&actionSetStartPattern, SIGNAL(triggered(bool)), this, SLOT(setStartPattern(bool)));
    QObject::connect(&actionRenamePattern, SIGNAL(triggered(bool)), this, SLOT(renamePattern(bool)));
    QObject::connect(&actionSetGoto, SIGNAL(triggered(bool)), this, SLOT(setGoto(bool)));
    QObject::connect(&actionRemoveGoto, SIGNAL(triggered(bool)), this, SLOT(removeGoto(bool)));
    QObject::connect(&actionMovePatternUp, SIGNAL(triggered(bool)), this, SLOT(movePatternUp(bool)));
    QObject::connect(&actionMovePatternDown, SIGNAL(triggered(bool)), this, SLOT(movePatternDown(bool)));
    QObject::connect(&actionInsertPatternBefore, SIGNAL(triggered(bool)), this, SLOT(insertPatternBefore(bool)));
    QObject::connect(&actionInsertPatternAfter, SIGNAL(triggered(bool)), this, SLOT(insertPatternAfter(bool)));
    QObject::connect(&actionRemovePattern, SIGNAL(triggered(bool)), this, SLOT(removePattern(bool)));
    QObject::connect(&actionDuplicatePattern, SIGNAL(triggered(bool)), this, SLOT(duplicatePattern(bool)));

    // Channel context menu
    QObject::connect(&actionSlide, SIGNAL(triggered(bool)), this, SLOT(setSlideValue(bool)));
    QObject::connect(&actionSetFrequency, SIGNAL(triggered(bool)), this, SLOT(setFrequency(bool)));
    QObject::connect(&actionHold, SIGNAL(triggered(bool)), this, SLOT(setHold(bool)));
    QObject::connect(&actionPause, SIGNAL(triggered(bool)), this, SLOT(setPause(bool)));
    QObject::connect(&actionDeleteRow, SIGNAL(triggered(bool)), this, SLOT(deleteRow(bool)));
    QObject::connect(&actionInsertRowBefore, SIGNAL(triggered(bool)), this, SLOT(insertRowBefore(bool)));
    QObject::connect(&actionInsertRowAfter, SIGNAL(triggered(bool)), this, SLOT(insertRowAfter(bool)));
    QObject::connect(&actionMuteChannel, SIGNAL(triggered(bool)), this, SLOT(toggleMute(bool)));

    editor->registerPatternMenu(&patternContextMenu);
    editor->registerChannelMenu(&channelContextMenu);
    timeline->registerPatternMenu(&patternContextMenu);
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

void TrackTab::toggleFollow(bool) {
    QCheckBox *cb = findChild<QCheckBox *>("checkBoxFollow");
    cb->toggle();
}

/*************************************************************************/

void TrackTab::setStartPattern(bool) {
    pTrack->lock();
    pTrack->startPatterns[contextEventChannel] = pTrack->getSequenceEntryIndex(contextEventChannel, contextEventNoteIndex);
    pTrack->unlock();
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
        pTrack->lock();
        entry->gotoTarget = dialog.getGotoValue() - 1;
        pTrack->unlock();
        update();
    }

}

/*************************************************************************/

void TrackTab::removeGoto(bool) {
    pTrack->lock();
    int entryIndex = pTrack->getSequenceEntryIndex(contextEventChannel, contextEventNoteIndex);
    Track::SequenceEntry *entry = &(pTrack->channelSequences[contextEventChannel].sequence[entryIndex]);
    entry->gotoTarget = -1;
    pTrack->unlock();
    update();
}

/*************************************************************************/

void TrackTab::movePatternUp(bool) {
    int entryIndex = pTrack->getSequenceEntryIndex(contextEventChannel, contextEventNoteIndex);
    if (entryIndex > 0) {
        emit stopTrack();
        pTrack->channelSequences[contextEventChannel].sequence.swap(entryIndex, entryIndex - 1);
        pTrack->updateFirstNoteNumbers();
        update();
    }
}

/*************************************************************************/

void TrackTab::movePatternDown(bool) {
    int entryIndex = pTrack->getSequenceEntryIndex(contextEventChannel, contextEventNoteIndex);
    if (entryIndex != pTrack->channelSequences[contextEventChannel].sequence.size() - 1) {
        emit stopTrack();
        pTrack->channelSequences[contextEventChannel].sequence.swap(entryIndex, entryIndex + 1);
        pTrack->updateFirstNoteNumbers();
        update();
    }
}

/*************************************************************************/

void TrackTab::insertPatternBefore(bool) {
    emit stopTrack();
    int patternIndex = choosePatternToInsert(true);
    if (patternIndex == -1) {
        return;
    }
    int entryIndex = pTrack->getSequenceEntryIndex(contextEventChannel, contextEventNoteIndex);
    Track::SequenceEntry newEntry(patternIndex);
    pTrack->channelSequences[contextEventChannel].sequence.insert(entryIndex, newEntry);
    pTrack->updateFirstNoteNumbers();
    update();
}

/*************************************************************************/

void TrackTab::insertPatternAfter(bool) {
    emit stopTrack();
    int patternIndex = choosePatternToInsert(false);
    if (patternIndex == -1) {
        return;
    }
    int entryIndex = pTrack->getSequenceEntryIndex(contextEventChannel, contextEventNoteIndex);
    Track::SequenceEntry newEntry(patternIndex);
    pTrack->channelSequences[contextEventChannel].sequence.insert(entryIndex + 1, newEntry);
    pTrack->updateFirstNoteNumbers();
    update();
}

/*************************************************************************/

void TrackTab::removePattern(bool) {
    emit stopTrack();
    if (pTrack->channelSequences[contextEventChannel].sequence.size() == 1) {
        MainWindow::displayMessage("A channel must contain at least one pattern!");
        return;
    }

    int entryIndex = pTrack->getSequenceEntryIndex(contextEventChannel, contextEventNoteIndex);
    int patternIndex = pTrack->channelSequences[contextEventChannel].sequence[entryIndex].patternIndex;
    pTrack->channelSequences[contextEventChannel].sequence.removeAt(entryIndex);
    pTrack->updateFirstNoteNumbers();
    // Check if pattern is no longer used anywhere
    bool wasLast = true;
    for (int channel = 0; channel < 2 && wasLast; ++channel) {
        for (int i = 0; i < pTrack->channelSequences[channel].sequence.size(); ++i) {
            if (pTrack->channelSequences[channel].sequence[i].patternIndex == patternIndex) {
                wasLast = false;
                break;
            }
        }
    }
    if (wasLast) {
        QMessageBox msgBox(QMessageBox::NoIcon,
                           "Delete Pattern?",
                           "This pattern is no longer used in the track. Do you want to delete it?",
                           QMessageBox::Yes | QMessageBox::No, this,
                           Qt::FramelessWindowHint);
        if (msgBox.exec() == QMessageBox::Yes) {
            // Correct SequenceEntries
            for (int channel = 0; channel < 2; ++channel) {
                for (int i = 0; i < pTrack->channelSequences[channel].sequence.size(); ++i) {
                    if (pTrack->channelSequences[channel].sequence[i].patternIndex > patternIndex) {
                        pTrack->channelSequences[channel].sequence[i].patternIndex--;
                    }
                }
            }
            pTrack->patterns.removeAt(patternIndex);
            pTrack->updateFirstNoteNumbers();
        }
    }
    // Validate gotos
    for (int i = 0; i < pTrack->channelSequences[contextEventChannel].sequence.size(); ++i) {
        if (pTrack->channelSequences[contextEventChannel].sequence[i].gotoTarget >= entryIndex) {
            pTrack->channelSequences[contextEventChannel].sequence[i].gotoTarget--;
        }
    }

    update();
}

/*************************************************************************/

void TrackTab::duplicatePattern(bool) {
    emit stopTrack();
    RenamePatternDialog dialog(this);
    int entryIndex = pTrack->getSequenceEntryIndex(contextEventChannel, contextEventNoteIndex);
    int patternIndex = pTrack->channelSequences[contextEventChannel].sequence[entryIndex].patternIndex;
    dialog.setPatternName(pTrack->patterns[patternIndex].name);
    if (dialog.exec() == QDialog::Accepted) {
        pTrack->patterns.append(pTrack->patterns[patternIndex]);
        pTrack->patterns.last().name = dialog.getPatternName();
        update();
    }
}

/*************************************************************************/

void TrackTab::setSlideValue(bool) {
    emit stopTrack();
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
        emit advanceEditPos();
        updatePatternEditor();
    }
}

/*************************************************************************/

void TrackTab::setFrequency(bool) {
    emit stopTrack();
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
        emit advanceEditPos();
        updatePatternEditor();
    }
}

/*************************************************************************/

void TrackTab::setHold(bool) {
    pTrack->lock();
    pTrack->getNote(contextEventChannel, contextEventNoteIndex)->type = Track::Note::instrumentType::Hold;
    pTrack->unlock();
    emit advanceEditPos();
    update();
}

/*************************************************************************/

void TrackTab::setPause(bool) {
    pTrack->lock();
    pTrack->getNote(contextEventChannel, contextEventNoteIndex)->type = Track::Note::instrumentType::Pause;
    int patternIndex = pTrack->getPatternIndex(contextEventChannel, contextEventNoteIndex);
    pTrack->validatePattern(patternIndex);
    pTrack->unlock();
    emit advanceEditPos();
    update();

}

/*************************************************************************/

void TrackTab::deleteRow(bool) {
    emit stopTrack();
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
    emit validateEditPos();
    update();
}

/*************************************************************************/

void TrackTab::insertRowBefore(bool) {
    emit stopTrack();
    int patternIndex = pTrack->getPatternIndex(contextEventChannel, contextEventNoteIndex);
    Track::Pattern *pattern = &(pTrack->patterns[patternIndex]);
    if (pattern->notes.size() == Track::Pattern::maxSize) {
        MainWindow::displayMessage("Pattern is already at maximum size!");
        return;
    }

    int noteInPattern = pTrack->getNoteIndexInPattern(contextEventChannel, contextEventNoteIndex);
    Track::Note newNote(Track::Note::instrumentType::Hold, 0, 0);
    pattern->notes.insert(noteInPattern, newNote);
    pTrack->updateFirstNoteNumbers();
    pTrack->validatePattern(patternIndex);
    update();
}

/*************************************************************************/

void TrackTab::insertRowAfter(bool) {
    emit stopTrack();
    int patternIndex = pTrack->getPatternIndex(contextEventChannel, contextEventNoteIndex);
    Track::Pattern *pattern = &(pTrack->patterns[patternIndex]);
    if (pattern->notes.size() == Track::Pattern::maxSize) {
        MainWindow::displayMessage("Pattern is already at maximum size!");
        return;
    }

    int noteInPattern = pTrack->getNoteIndexInPattern(contextEventChannel, contextEventNoteIndex);
    Track::Note newNote(Track::Note::instrumentType::Hold, 0, 0);
    pattern->notes.insert(noteInPattern + 1, newNote);
    pTrack->updateFirstNoteNumbers();
    pTrack->validatePattern(patternIndex);
    update();
}

/*************************************************************************/

void TrackTab::toggleMute(bool) {
    isMuted[contextEventChannel] = !isMuted[contextEventChannel];
    emit setMuted(contextEventChannel, isMuted[contextEventChannel]);

}

/*************************************************************************/

void TrackTab::invalidNoteFound(int channel, int entryIndex, int noteIndex) {
    // TODO: Create sensible error string
    MainWindow::displayMessage("Invalid note found!");
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

/*************************************************************************/

int TrackTab::choosePatternToInsert(bool doBefore) {
    InsertPatternDialog dialog(this);
    dialog.prepare(pTrack);
    if (dialog.exec() == QDialog::Accepted) {
        int result = dialog.getSelectedPattern();
        // Check for "create new pattern"
        if (result == pTrack->patterns.size()) {
            // Calc index of first note of new pattern, for "align" button
            int entryIndex = pTrack->getSequenceEntryIndex(contextEventChannel, contextEventNoteIndex);
            int patternIndex = pTrack->channelSequences[contextEventChannel].sequence[entryIndex].patternIndex;
            int newRow = pTrack->channelSequences[contextEventChannel].sequence[entryIndex].firstNoteNumber;
            if (!doBefore) {
                newRow += pTrack->patterns[patternIndex].notes.size();
            }
            CreatePatternDialog newDialog(this);
            newDialog.prepare(pTrack, lastNewPatternLength, contextEventChannel, newRow);
            if (newDialog.exec() == QDialog::Accepted) {
                QString newName = newDialog.getName();
                int newLength = newDialog.getLength();
                Track::Pattern newPattern(newName);
                for (int i = 0; i < newLength; ++i) {
                    Track::Note newNote(Track::Note::instrumentType::Hold, 0, 0);
                    newPattern.notes.append(newNote);
                }
                pTrack->patterns.append(newPattern);
                lastNewPatternLength = newLength;
            } else {
                return -1;
            }
        }
        return result;
    } else {
        return -1;
    }
}

/*************************************************************************/

void TrackTab::addShortcut(QAction *action, QString actionName) {
    if (MainWindow::keymap.contains(actionName)) {
        QString shortcut = MainWindow::keymap[actionName].toString();
        action->setShortcut(QKeySequence(shortcut));
        addAction(action);
    }
}
