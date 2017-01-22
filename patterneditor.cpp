/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

#include "patterneditor.h"
#include <QFontMetrics>
#include <QPainter>
#include "mainwindow.h"
#include "track/pattern.h"
#include "track/sequence.h"
#include "track/sequenceentry.h"
#include "track/note.h"
#include "tiasound/pitchguidefactory.h"
#include "tiasound/pitchguide.h"
#include "tiasound/instrumentpitchguide.h"
#include "tiasound/tiasound.h"
#include <QWheelEvent>


PatternEditor::PatternEditor(QWidget *parent) : QWidget(parent)
{
    legendFont.setPixelSize(legendFontSize);
    QFontMetrics legendFontMetrics(legendFont);
    legendFontHeight = legendFontMetrics.height();
    timeAreaWidth = legendFontMetrics.width("000:00");

    noteFont.setPixelSize(noteFontSize);
    QFontMetrics noteFontMetrics(noteFont);
    noteFontHeight = noteFontMetrics.height();
    noteAreaWidth = noteFontMetrics.width("000: C#4 I7 31")
            + 2*noteMargin;

    widgetWidth = 2*patternNameWidth
            + 2*noteAreaWidth
            + timeAreaWidth;
    setFixedWidth(widgetWidth);

    setFocusPolicy(Qt::StrongFocus);
}

/*************************************************************************/

void PatternEditor::registerTrack(Track::Track *newTrack) {
    pTrack = newTrack;
}

/*************************************************************************/

void PatternEditor::registerPitchGuide(TiaSound::PitchGuide *newGuide) {
    pPitchGuide = newGuide;
}

/*************************************************************************/

void PatternEditor::registerPlayer(Emulation::Player *newPlayer) {
    pPlayer = newPlayer;
}

/*************************************************************************/

void PatternEditor::registerMuteAction(QAction *newAction) {
    muteAction = newAction;
}

/*************************************************************************/

void PatternEditor::registerPatternMenu(QMenu *newPatternMenu) {
    pPatternMenu = newPatternMenu;
}

/*************************************************************************/

void PatternEditor::registerChannelMenu(QMenu *newChannelMenu) {
    pChannelMenu = newChannelMenu;
}

/*************************************************************************/

void PatternEditor::registerInstrumentSelector(InstrumentSelector *selector) {
    pInsSelector = selector;
}

/*************************************************************************/

void PatternEditor::setEditPos(int newPos) {
    editPos = newPos;
    if (editPos < 0) {
        editPos = 0;
    }
    if (editPos >= pTrack->getChannelNumRows(selectedChannel)) {
        editPos = pTrack->getChannelNumRows(selectedChannel) - 1;
    }
    emit editPosChanged(editPos);
    emit channelContextEvent(selectedChannel, editPos);
    update();
}

/*************************************************************************/

void PatternEditor::setEditPos(int newChannel, int newPos) {
    selectedChannel = newChannel;
    setEditPos(newPos);
}

/*************************************************************************/

void PatternEditor::validateEditPos() {
    setEditPos(editPos);
}

/*************************************************************************/

void PatternEditor::advanceEditPos() {
    setEditPos(editPos + 1);
}

/*************************************************************************/

void PatternEditor::setRowsPerBeat(int value) {
    pTrack->rowsPerBeat = value;
    update();
}

/*************************************************************************/

void PatternEditor::setRowToInstrument(int frequency) {
    int patternIndex = pTrack->getPatternIndex(selectedChannel, editPos);
    int noteIndex = pTrack->getNoteIndexInPattern(selectedChannel, editPos);
    int instrumentIndex = pInsSelector->getSelectedInstrument();
    if (instrumentIndex < 7) {
        // Meldodic instrument
        pTrack->patterns[patternIndex].notes[noteIndex].type = Track::Note::instrumentType::Instrument;
        pTrack->patterns[patternIndex].notes[noteIndex].instrumentNumber = instrumentIndex;
        pTrack->patterns[patternIndex].notes[noteIndex].value = frequency;
    } else {
        // Percussion instrument
        pTrack->patterns[patternIndex].notes[noteIndex].type = Track::Note::instrumentType::Percussion;
        pTrack->patterns[patternIndex].notes[noteIndex].instrumentNumber = instrumentIndex - Track::Track::numInstruments;
    }
    advanceEditPos();
    update();
}

/*************************************************************************/

void PatternEditor::newPlayerPos(int pos1, int pos2) {
    if (follow) {
        if (selectedChannel == 0) {
            setEditPos(pos1);
        } else {
            setEditPos(pos2);
        }
    }
}

/*************************************************************************/

void PatternEditor::toggleFollow_clicked(bool toggle) {
    follow = toggle;
}

/*************************************************************************/

void PatternEditor::toggleLoop_clicked(bool toggle) {
    loop = toggle;
}

/*************************************************************************/

int PatternEditor::getEditPos() {
    return editPos;
}

/*************************************************************************/

int PatternEditor::getSelectedChannel() {
    return selectedChannel;
}

/*************************************************************************/

QSize PatternEditor::sizeHint() const {
    return QSize(widgetWidth, minHeight);
}

/*************************************************************************/

void PatternEditor::moveUp(bool) {
    setEditPos(editPos - 1);
}

/*************************************************************************/

void PatternEditor::moveDown(bool) {
    setEditPos(editPos + 1);
}

/*************************************************************************/

void PatternEditor::moveLeft(bool) {
    selectedChannel = 0;
    setEditPos(editPos);
    emit editChannelChanged(selectedChannel);
}

/*************************************************************************/

void PatternEditor::moveRight(bool) {
    selectedChannel = 1;
    setEditPos(editPos);
    emit editChannelChanged(selectedChannel);
}

/*************************************************************************/

void PatternEditor::switchChannel(bool) {
    selectedChannel = 1 - selectedChannel;
    setEditPos(editPos);
    emit editChannelChanged(selectedChannel);
}

/*************************************************************************/

void PatternEditor::gotoFirstRow(bool) {
    setEditPos(0);
}

/*************************************************************************/

void PatternEditor::gotoLastRow(bool) {
    setEditPos(pTrack->getChannelNumRows(selectedChannel) - 1);
}

/*************************************************************************/

void PatternEditor::gotoNextPattern(bool) {
    int newEntryIndex = pTrack->getSequenceEntryIndex(selectedChannel, editPos) + 1;
    if (newEntryIndex == pTrack->channelSequences[selectedChannel].sequence.size()) {
        newEntryIndex--;
    }
    int newPos = pTrack->channelSequences[selectedChannel].sequence[newEntryIndex].firstNoteNumber;
    setEditPos(newPos);
}

/*************************************************************************/

void PatternEditor::gotoPreviousPattern(bool) {
    int newEntryIndex = pTrack->getSequenceEntryIndex(selectedChannel, editPos) - 1;
    if (newEntryIndex < 0) {
        newEntryIndex = 0;
    }
    int newPos = pTrack->channelSequences[selectedChannel].sequence[newEntryIndex].firstNoteNumber;
    setEditPos(newPos);
}

/*************************************************************************/

QString PatternEditor::constructRowString(int curPatternNoteIndex, Track::Pattern *curPattern) {
    QString rowText = QString::number(curPatternNoteIndex + 1);
    if (curPatternNoteIndex + 1 < 10) {
        rowText.prepend("  ");
    } else if (curPatternNoteIndex < 100) {
        rowText.prepend(" ");
    }
    switch (curPattern->notes[curPatternNoteIndex].type) {
    case Track::Note::instrumentType::Hold:
        rowText.append(":    |");
        break;
    case Track::Note::instrumentType::Slide: {
        int frequency = curPattern->notes[curPatternNoteIndex].value;
        rowText.append(":   ");
        rowText.append("  SL");
        // Frequency change
        if (frequency < 0) {
            rowText.append(" ");
        } else {
            rowText.append(" +");
        }
        rowText.append(QString::number(frequency));
        break;
    }
    case Track::Note::instrumentType::Pause:
        rowText.append(":   ---");
        break;
    case Track::Note::instrumentType::Percussion: {
        int percNum = curPattern->notes[curPatternNoteIndex].instrumentNumber + 1;
        if (percNum < 10) {
            rowText.append(":   P ");
        } else {
            rowText.append(":   P");
        }
        rowText.append(QString::number(percNum));
        break;
    }
    case Track::Note::instrumentType::Instrument: {
        int insNum = curPattern->notes[curPatternNoteIndex].instrumentNumber;
        // Pitch
        int frequency = curPattern->notes[curPatternNoteIndex].value;
        TiaSound::Distortion dist = pTrack->instruments[insNum].baseDistortion;
        // In case the instrument got changed from PURE_COMBINED to something else
        if (frequency > 31 && dist != TiaSound::Distortion::PURE_COMBINED) {
            frequency -= 32;
        }
        TiaSound::InstrumentPitchGuide *pIPG = &(pPitchGuide->instrumentGuides[dist]);
        TiaSound::Note note = pIPG->getNote(frequency);
        if (note == TiaSound::Note::NotANote) {
            rowText.append(": ???");
        } else {
            rowText.append(": ");
            rowText.append(TiaSound::getNoteNameWithOctaveFixedWidth(note));
        }
        // Instrument number
        rowText.append(" I");
        rowText.append(QString::number(insNum + 1));
        // Frequency
        if (frequency < 10) {
            rowText.append("  ");
        } else {
            rowText.append(" ");
        }
        rowText.append(QString::number(frequency));
        break;
    }
    default:
        rowText.append(": ??? ");
        break;
    }

    return rowText;
}

void PatternEditor::drawPatternNameAndSeparator(int yPos, int nameXPos, int curPatternNoteIndex, int channel, int xPos, int curEntryIndex, QPainter *painter, Track::Pattern *curPattern)
{
    if (curPatternNoteIndex == 0) {
        painter->fillRect(xPos - noteMargin, yPos, noteAreaWidth, 1, MainWindow::contentDarker);
        painter->setFont(legendFont);
        painter->setPen(MainWindow::contentDarker);
        int alignment = channel == 0 ? Qt::AlignRight : Qt::AlignLeft;
        QString patternName = QString::number(curEntryIndex + 1);
        patternName.append(": ");
        patternName.append(curPattern->name);
        if (curEntryIndex == pTrack->startPatterns[channel]) {
            painter->setPen(MainWindow::green);

        } else {
            painter->setPen(MainWindow::blue);
        }
        painter->drawText(nameXPos, yPos, patternNameWidth - 2*patternNameMargin, legendFontHeight, alignment, patternName);
    }
}

void PatternEditor::drawGoto(int channel, int yPos, Track::Pattern *curPattern, Track::SequenceEntry *curEntry, QPainter *painter, int nameXPos, int curPatternNoteIndex)
{
    if (curPatternNoteIndex == curPattern->notes.size() - 1
            && curEntry->gotoTarget != -1) {
        int alignment = channel == 0 ? Qt::AlignRight : Qt::AlignLeft;
        painter->setFont(legendFont);
        painter->setPen(MainWindow::blue);
        painter->drawText(nameXPos, yPos, patternNameWidth - 2*patternNameMargin, legendFontHeight, alignment,
                          "GOTO " + QString::number(curEntry->gotoTarget + 1));
    }
}

void PatternEditor::drawTimestamp(int row, QPainter *painter, int yPos, int channel)
{
    if (pTrack->globalSpeed) {
        int ticksPerSecond = pTrack->getTvMode() == TiaSound::TvStandard::PAL ? 50 : 60;
        long numOddTicks = int((row + 1)/2)*pTrack->oddSpeed;
        long numEvenTicks = int(row/2)*pTrack->evenSpeed;
        long numTick = numOddTicks + numEvenTicks;
        int curTicks = row%2 == 0 ? pTrack->evenSpeed : pTrack->oddSpeed;
        if (channel == 0 && numTick%ticksPerSecond < curTicks) {
            int minute = numTick/(ticksPerSecond*60);
            int second = (numTick%(ticksPerSecond*60))/ticksPerSecond;
            QString timestampText = QString::number(minute);
            if (second < 10) {
                timestampText.append(":0");
            } else {
                timestampText.append(":");
            }
            timestampText.append(QString::number(second));
            painter->setFont(legendFont);
            painter->setPen(MainWindow::contentDarker);
            painter->drawText(patternNameWidth + noteAreaWidth, yPos, timeAreaWidth, legendFontHeight, Qt::AlignHCenter, timestampText);
        }
    }
}

void PatternEditor::paintChannel(QPainter *painter, int channel, int xPos, int nameXPos) {
    // Calc first note/pattern
    int firstNoteIndex = max(0, editPos - numRows/2);
    // Don't do anything if we are behind the last note
    int channelSize = pTrack->getChannelNumRows(channel);
    if (firstNoteIndex >= channelSize) {
        return;
    }
    // Get pointers to first note to paint
    int curEntryIndex = 0;
    Track::SequenceEntry *curEntry = &(pTrack->channelSequences[channel].sequence[0]);
    Track::Pattern *curPattern = &(pTrack->patterns[curEntry->patternIndex]);
    while (firstNoteIndex >= curEntry->firstNoteNumber + curPattern->notes.size()) {
        curEntryIndex++;
        curEntry = &(pTrack->channelSequences[channel].sequence[curEntryIndex]);
        curPattern = &(pTrack->patterns[curEntry->patternIndex]);
    }
    int curPatternNoteIndex = firstNoteIndex - curEntry->firstNoteNumber;
    // Draw rows
    for (int row = firstNoteIndex; row <= editPos + numRows/2; ++row) {
        int yPos = topMargin + noteFontHeight*(row - (editPos - numRows/2));
        // First row in beat?
        if (row%(pTrack->rowsPerBeat) == 0 && (channel != selectedChannel || row != editPos)) {
            painter->fillRect(xPos - noteMargin, yPos, noteAreaWidth, noteFontHeight, MainWindow::darkHighlighted);
        }
        QString rowText = constructRowString(curPatternNoteIndex, curPattern);
        painter->setFont(noteFont);
        painter->setPen(MainWindow::blue);
        painter->drawText(xPos, yPos, noteAreaWidth - 2*noteMargin, noteFontHeight, Qt::AlignLeft, rowText);

        drawPatternNameAndSeparator(yPos, nameXPos, curPatternNoteIndex, channel, xPos, curEntryIndex, painter, curPattern);
        drawGoto(channel, yPos, curPattern, curEntry, painter, nameXPos, curPatternNoteIndex);
        drawTimestamp(row, painter, yPos, channel);

        // Advance note
        if (!pTrack->getNextNote(channel, &curEntryIndex, &curPatternNoteIndex)) {
            // End of track reached: Stop drawing
            break;
        }
        curEntry = &(pTrack->channelSequences[channel].sequence[curEntryIndex]);
        curPattern = &(pTrack->patterns[curEntry->patternIndex]);
    }
}

void PatternEditor::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    // Pattern name areas
    painter.fillRect(0, 0, patternNameWidth, height(), MainWindow::lightHighlighted);
    painter.fillRect(widgetWidth - patternNameWidth, 0, patternNameWidth, height(), MainWindow::lightHighlighted);
    // Note areas
    painter.fillRect(patternNameWidth, 0, noteAreaWidth, height(), MainWindow::dark);
    painter.fillRect(patternNameWidth + noteAreaWidth + timeAreaWidth, 0, noteAreaWidth, height(), MainWindow::dark);
    // Time area
    painter.fillRect(patternNameWidth + noteAreaWidth, 0, timeAreaWidth, height(), MainWindow::lightHighlighted);
    // Current highlights
    int highlightY = height()/2 - noteFontHeight/2;
    int highlightX = patternNameWidth + selectedChannel*(noteAreaWidth + timeAreaWidth);
    painter.fillRect(highlightX, highlightY, noteAreaWidth, noteFontHeight, MainWindow::light);

    // Calc number of visible rows
    numRows = height()/noteFontHeight;
    if (numRows%2 == 0) {
        numRows--;
    }
    topMargin = (height() - numRows*noteFontHeight)/2;

    // Paint channels
    paintChannel(&painter, 0, patternNameWidth + noteMargin, patternNameMargin);
    paintChannel(&painter, 1, patternNameWidth + noteAreaWidth + timeAreaWidth + noteMargin, width() - patternNameWidth + patternNameMargin);

}

/*************************************************************************/

void PatternEditor::wheelEvent(QWheelEvent *event) {
    int newPos = editPos - event->delta()/100;
    setEditPos(newPos);
}

/*************************************************************************/

bool PatternEditor::clickedInValidRow(int x, int y, int *channel, int *noteIndex) {
    // Do nothing if we are outside a valid row
    if (y < topMargin || y > topMargin + numRows*noteFontHeight) {
        return false;
    }
    int row = (y - topMargin)/noteFontHeight;
    *noteIndex = editPos - (numRows/2 - row);
    if (x < patternNameWidth + noteAreaWidth) {
        *channel = 0;
    } else  if (x >= patternNameWidth + noteAreaWidth + timeAreaWidth) {
        *channel = 1;
    } else {
        return false;
    }
    int channelSize = pTrack->getChannelNumRows(*channel);
    if (*noteIndex < 0 || *noteIndex >= channelSize) {
        return false;
    }
    return true;
}


/*************************************************************************/

void PatternEditor::mousePressEvent(QMouseEvent *event) {
    if (event->button() != Qt::LeftButton) {
        return;
    }
    int channel;
    int noteIndex;
    if (!clickedInValidRow(event->x(), event->y(), &channel, &noteIndex)) {
        return;
    }
    selectedChannel = channel;
    setEditPos(noteIndex);
    emit editChannelChanged(selectedChannel);
}

/*************************************************************************/

void PatternEditor::contextMenuEvent(QContextMenuEvent *event) {
    int channel;
    int noteIndex;
    if (!clickedInValidRow(event->x(), event->y(), &channel, &noteIndex)) {
        return;
    }

    // Set correct mute toggle state
    muteAction->setChecked(pPlayer->channelMuted[channel]);
    // Determine correct context menu to display
    if (event->x() < patternNameWidth) {
        emit channelContextEvent(channel, noteIndex);
        pPatternMenu->exec(event->globalPos());
    } else if (event->x() >= patternNameWidth && event->x() < patternNameWidth + noteAreaWidth) {
        emit channelContextEvent(channel, noteIndex);
        pChannelMenu->exec(event->globalPos());
    } else if (event->x() >= patternNameWidth + noteAreaWidth + timeAreaWidth
               && event->x() < patternNameWidth + noteAreaWidth + timeAreaWidth + noteAreaWidth) {
        emit channelContextEvent(channel, noteIndex);
        pChannelMenu->exec(event->globalPos());
    } else if (event->x() >= patternNameWidth + noteAreaWidth + timeAreaWidth + noteAreaWidth
               && event->x() < patternNameWidth + noteAreaWidth + timeAreaWidth + noteAreaWidth + patternNameWidth){
        emit channelContextEvent(channel, noteIndex);
        pPatternMenu->exec(event->globalPos());
    }
}
