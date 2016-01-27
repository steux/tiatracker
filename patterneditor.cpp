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

QSize PatternEditor::sizeHint() const {
    return QSize(widgetWidth, minHeight);
}

/*************************************************************************/

void PatternEditor::paintChannel(QPainter *painter, int channel, int xPos, int yOffset, int numRows, int nameXPos) {
    // Calc first note/pattern
    int firstNoteIndex = max(0, editPos - numRows/2);
    int curEntryIndex = 0;
    Track::SequenceEntry *curEntry = &(pTrack->channelSequences[channel].sequence[0]);
    Track::Pattern *curPattern = &(pTrack->patterns[curEntry->patternIndex]);
    while (firstNoteIndex >= curEntry->firstNoteNumber + curPattern->notes.size()) {
        curEntryIndex++;
        curEntry = &(pTrack->channelSequences[channel].sequence[curEntryIndex]);
        curPattern = &(pTrack->patterns[curEntry->patternIndex]);
    }
    int curPatternNoteIndex = firstNoteIndex - curEntry->firstNoteNumber;
    int ticksPerSecond = pTrack->getTvMode() == TiaSound::TvStandard::PAL ? 50 : 60;

    // Draw rows
    for (int row = firstNoteIndex; row <= editPos + numRows/2; ++row) {
        int yPos = yOffset + noteFontHeight*(row - (editPos - numRows/2));
        // First row in beat?
        if (row%rowsPerBeat == 0 && (channel != 0 || row != editPos)) {
            painter->fillRect(xPos - noteMargin, yPos, noteAreaWidth, noteFontHeight, MainWindow::darkHighlighted);
        }
        // Construct row string
        QString rowText = QString::number(curPatternNoteIndex + 1);
        if (curPatternNoteIndex < 10) {
            rowText.prepend("  ");
        } else if (curPatternNoteIndex < 100) {
            rowText.prepend(" ");
        }
        switch (curPattern->notes[curPatternNoteIndex].type) {
        case Track::Note::instrumentType::Hold:
            rowText.append(":    |");
            break;
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
            int insNum = curPattern->notes[curPatternNoteIndex].instrumentNumber + 1;
            // Pitch
            int frequency = curPattern->notes[curPatternNoteIndex].value;
            TiaSound::Distortion dist = pTrack->instruments[insNum].baseDistortion;
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
            rowText.append(QString::number(insNum));
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

        painter->setFont(noteFont);
        painter->setPen(MainWindow::blue);
        painter->drawText(xPos, yPos, noteAreaWidth - 2*noteMargin, noteFontHeight, Qt::AlignLeft, rowText);
        // Draw pattern name and pattern separator?
        if (curPatternNoteIndex == 0) {
            painter->fillRect(xPos - noteMargin, yPos, noteAreaWidth, 1, MainWindow::contentDarker);
            painter->setFont(legendFont);
            painter->setPen(MainWindow::contentDarker);
            int alignment = channel == 0 ? Qt::AlignRight : Qt::AlignLeft;
            painter->drawText(nameXPos, yPos, patternNameWidth - 2*patternNameMargin, legendFontHeight, alignment, curPattern->name);
        }
        // Draw timestamp?
        long numTick = row*pTrack->speed;
        if (channel == 0 && numTick%ticksPerSecond < pTrack->speed) {
            int minute = numTick/(ticksPerSecond*60);
            int second = numTick/ticksPerSecond;
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

        // Advance note
        curPatternNoteIndex++;
        if (curPatternNoteIndex == curPattern->notes.size()) {
            curEntryIndex++;
            if (curEntryIndex == pTrack->channelSequences[channel].sequence.size()) {
                // End of track reached; stop drawing
                break;
            }
            curEntry = &(pTrack->channelSequences[channel].sequence[curEntryIndex]);
            curPattern = &(pTrack->patterns[curEntry->patternIndex]);
            curPatternNoteIndex = 0;
        }
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
    painter.fillRect(patternNameWidth, highlightY, noteAreaWidth, noteFontHeight, MainWindow::light);
    //painter.fillRect(patternNameWidth + noteAreaWidth + timeAreaWidth, highlightY, noteAreaWidth, noteFontHeight, MainWindow::darkHighlighted);

    // Calc number of visible rows
    int numRows = height()/noteFontHeight;
    if (numRows%2 == 0) {
        numRows--;
    }
    int topMargin = (height() - numRows*noteFontHeight)/2;

    // Paint channels
    paintChannel(&painter, 0, patternNameWidth + noteMargin, topMargin, numRows, patternNameMargin);
    paintChannel(&painter, 1, patternNameWidth + noteAreaWidth + timeAreaWidth + noteMargin, topMargin, numRows, width() - patternNameWidth + patternNameMargin);

}

/*************************************************************************/

void PatternEditor::wheelEvent(QWheelEvent *event) {
    editPos -= event->delta()/100;
    if (editPos < 0) {
        editPos = 0;
    }
    if (editPos >= pTrack->getNumRows()) {
        editPos = pTrack->getNumRows() - 1;
    }
    update();
}
