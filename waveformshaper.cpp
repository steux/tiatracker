#include "waveformshaper.h"
#include <QPainter>
#include "mainwindow.h"
#include <cassert>
#include <QMouseEvent>

WaveformShaper::WaveformShaper(QWidget *parent) : QWidget(parent)
{
    valueFont.setPixelSize(valueFontSize);
    QFontMetrics fontMetrics(valueFont);
    valueFontHeight = fontMetrics.height();
    valueAreaHeight = valueFontHeight + 2*valueAreaMargin;
    setFixedWidth(calcWidth());
    setFixedHeight(valueAreaHeight + legendCellSize);
}

/*************************************************************************/

void WaveformShaper::registerPercussion(Track::Percussion *newPercussion) {
    pPercussion = newPercussion;

}

/*************************************************************************/

void WaveformShaper::updateSize() {
    setFixedWidth(calcWidth());
}

/*************************************************************************/

QList<TiaSound::Distortion> *WaveformShaper::getValues() {
    return values;
}

void WaveformShaper::setValues(QList<TiaSound::Distortion> *newValues) {
    values = newValues;
    updateSize();
}

/*************************************************************************/

void WaveformShaper::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    const int envelopeLength = pPercussion->getEnvelopeLength();

    // Left side
    painter.fillRect(0, 0, legendCellSize, valueAreaHeight+legendCellSize, MainWindow::lightHighlighted);
    // Bottom
    painter.fillRect(legendCellSize, valueAreaHeight, envelopeLength*cellWidth, legendCellSize, MainWindow::lightHighlighted);
    // Value area
    painter.fillRect(legendCellSize, 0, envelopeLength*cellWidth, valueAreaHeight, MainWindow::dark);
    // Frame numbers
    painter.setFont(valueFont);
    painter.setPen(MainWindow::contentDarker);
    for (int frame = 0; frame < envelopeLength; ++frame) {
        int xPos = legendCellSize + frame*cellWidth;
        painter.drawText(xPos, valueAreaHeight, cellWidth, legendCellSize, Qt::AlignCenter, QString::number(frame + 1));
    }
    // Values
    painter.setPen(MainWindow::contentLight);
    for (int iValue = 0; iValue < envelopeLength; ++iValue) {
        TiaSound::Distortion value = (*values)[iValue];
        int intValue = TiaSound::getDistortionInt(value);
        painter.drawText(legendCellSize + iValue*cellWidth, valueAreaMargin, cellWidth, valueFontHeight, Qt::AlignCenter, QString::number(intValue));
    }

}

/*************************************************************************/

int WaveformShaper::calcWidth() {
    int envelopeLength = 21;
    // During init, no percussion is registered yet
    if (pPercussion != nullptr) {
        envelopeLength = pPercussion->getEnvelopeLength();
    }
    int width = legendCellSize;
    if (values != nullptr) {
        width += envelopeLength*cellWidth;
    }
    return width;
}

/*************************************************************************/

