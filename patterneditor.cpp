#include "patterneditor.h"
#include <QFontMetrics>
#include <QPainter>
#include "mainwindow.h"
#include "track/pattern.h"
#include "track/sequence.h"
#include "track/sequenceentry.h"
#include "track/note.h"


PatternEditor::PatternEditor(QWidget *parent) : QWidget(parent)
{
    legendFont.setPixelSize(legendFontSize);
    QFontMetrics legendFontMetrics(legendFont);
    legendFontHeight = legendFontMetrics.height();
    timeAreaWidth = legendFontMetrics.width("99:99");

    noteFont.setPixelSize(noteFontSize);
    QFontMetrics noteFontMetrics(noteFont);
    noteFontHeight = noteFontMetrics.height();
    noteAreaWidth = noteFontMetrics.width("000: P15 C#4 31")
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

    // Draw rows
    for (int row = firstNoteIndex; row <= editPos + numRows/2; ++row) {
        int yPos = yOffset + noteFontHeight*(row - (editPos - numRows/2));
        QString rowText = QString::number(curPatternNoteIndex);
        if (curPatternNoteIndex < 10) {
            rowText.prepend("  ");
        } else if (curPatternNoteIndex < 100) {
            rowText.prepend(" ");
        }
        switch (curPattern->notes[curPatternNoteIndex].type) {
        case Track::Note::instrumentType::Hold:
            rowText.append(":  |   |");
            break;
        case Track::Note::instrumentType::Pause:
            rowText.append(": --- ");
            break;
        case Track::Note::instrumentType::Percussion: {
            int percNum = curPattern->notes[curPatternNoteIndex].instrumentNumber + 1;
            if (percNum < 10) {
                rowText.append(": P ");
            } else {
                rowText.append(": P");
            }
            rowText.append(QString::number(percNum));
            break;
        }
        case Track::Note::instrumentType::Instrument: {
            int insNum = curPattern->notes[curPatternNoteIndex].instrumentNumber + 1;
            rowText.append(": I ");
            rowText.append(QString::number(insNum));
            rowText.append(" C#4 19");
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
            int alignment;
            if (channel == 0) {
                alignment = Qt::AlignRight;
            } else {
                alignment = Qt::AlignLeft;
            }
            painter->drawText(nameXPos, yPos, patternNameWidth - 2*patternNameMargin, legendFontHeight, alignment, curPattern->name);
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
    painter.fillRect(patternNameWidth, highlightY, noteAreaWidth, noteFontHeight, MainWindow::darkHighlighted);
    painter.fillRect(patternNameWidth + noteAreaWidth + timeAreaWidth, highlightY, noteAreaWidth, noteFontHeight, MainWindow::darkHighlighted);

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

