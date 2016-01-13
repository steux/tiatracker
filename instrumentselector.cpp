#include "instrumentselector.h"
#include "track/track.h"
#include "mainwindow.h"
#include <QPainter>
#include <iostream>


InstrumentSelector::InstrumentSelector(QWidget *parent) : QWidget(parent)
{
    font.setPixelSize(fontSize);
    QFontMetrics fontMetrics(font);
    fontHeight = fontMetrics.height();
    buttonHeight = 2*buttonVerticalMargin + fontHeight;
    widgetHeight = 2*verticalMargin
            + buttonHeight*Track::Track::numInstruments
            + buttonHeight*Track::Track::numPercussion
            + insPercMargin;
    setFixedHeight(widgetHeight);
}

/*************************************************************************/

void InstrumentSelector::registerTrack(Track::Track *newTrack) {
    pTrack = newTrack;
}

/*************************************************************************/

QSize InstrumentSelector::sizeHint() const {
    return QSize(minSize, widgetHeight);
}

/*************************************************************************/

void InstrumentSelector::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    int buttonWidth = width() - 2*horizontalMargin - 2*buttonHorizontalMargin;

    painter.fillRect(0, 0, width(), height(), MainWindow::contentDark);
    // Buttons
    painter.setFont(font);
    painter.setPen(MainWindow::contentDark);
    for (int i = 0; i < Track::Track::numInstruments + Track::Track::numPercussion; ++i) {
        int yPos = verticalMargin + i*buttonHeight + buttonVerticalMargin;
        QString insName;
        if (i >= Track::Track::numInstruments) {
            // Percussion
            yPos += insPercMargin;
            insName = QString::number(i - Track::Track::numInstruments + 1) + ": " + pTrack->percussion[i - Track::Track::numInstruments].name;
        } else {
            // Instrument
            insName = QString::number(i + 1) + ": " + pTrack->instruments[i].name;
        }
        painter.fillRect(horizontalMargin, yPos, width() - 2*horizontalMargin, fontHeight, MainWindow::light);
        painter.drawText(horizontalMargin + buttonHorizontalMargin,
                         yPos + buttonVerticalMargin,
                         buttonWidth,
                         fontHeight,
                         Qt::AlignHCenter,
                         insName
                         );
    }
}

