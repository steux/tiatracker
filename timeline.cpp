#include "timeline.h"
#include <QPainter>
#include <QString>
#include <QFont>
#include "mainwindow.h"
#include "track/sequence.h"
#include "track/sequenceentry.h"
#include <QColor>


Timeline::Timeline(QWidget *parent) : QWidget(parent)
{
    setFixedWidth(2*channelWidth + channelMargin);
}

/*************************************************************************/

void Timeline::registerTrack(Track::Track *newTrack) {
    pTrack = newTrack;

    widgetWidth = 2*channelMargin + 2*channelWidth + channelGap;
    setFixedWidth(widgetWidth);
}

/*************************************************************************/

QSize Timeline::sizeHint() const {
    return QSize(widgetWidth, minHeight);
}

/*************************************************************************/

void Timeline::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    // Calc row height in pixels
    int lastEntry0 = pTrack->channelSequences[0].sequence.size() - 1;
    int lastPattern0 = pTrack->channelSequences[0].sequence[lastEntry0].patternIndex;
    int channelLength0 = pTrack->channelSequences[0].sequence[lastEntry0].firstNoteNumber
            + pTrack->patterns[lastPattern0].notes.size();
    int lastEntry1 = pTrack->channelSequences[1].sequence.size() - 1;
    int lastPattern1 = pTrack->channelSequences[1].sequence[lastEntry1].patternIndex;
    int channelLength1 = pTrack->channelSequences[1].sequence[lastEntry1].firstNoteNumber
            + pTrack->patterns[lastPattern1].notes.size();
    int maxLength = max(channelLength0, channelLength1);
    double rowHeight = (height() - 2*channelMargin)/double(maxLength);

    painter.fillRect(0, 0, width(), height(), MainWindow::dark);
    int xPos = channelMargin;
    for (int channel = 0; channel < 2; ++channel) {
        Track::Sequence *sequence = &(pTrack->channelSequences[channel]);
        for (int iEntry = 0; iEntry < sequence->sequence.size(); ++iEntry) {
            int patternTop = channelMargin + int(rowHeight*sequence->sequence[iEntry].firstNoteNumber + 0.5);
            int iPattern = sequence->sequence[iEntry].patternIndex;
            int patternHeight = int(rowHeight*pTrack->patterns[iPattern].notes.size() + 0.5);
            QColor col;
            if (iEntry%2 == 0) {
                col = MainWindow::contentDark;
            } else {
                col = MainWindow::contentLight;
            }
            painter.fillRect(xPos, patternTop, channelWidth, patternHeight, col);
        }
        xPos += channelWidth + channelGap;
    }

}

