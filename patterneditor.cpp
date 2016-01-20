#include "patterneditor.h"
#include <QFontMetrics>
#include <QPainter>
#include "mainwindow.h"


PatternEditor::PatternEditor(QWidget *parent) : QWidget(parent)
{
    legendFont.setPixelSize(legendFontSize);
    QFontMetrics legendFontMetrics(legendFont);
    legendFontHeight = legendFontMetrics.height();
    timeAreaWidth = legendFontMetrics.width("99:99");

    noteFont.setPixelSize(noteFontSize);
    QFontMetrics noteFontMetrics(legendFont);
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

}

