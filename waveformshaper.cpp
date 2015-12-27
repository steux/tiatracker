#include "waveformshaper.h"

WaveformShaper::WaveformShaper(QWidget *parent) : QWidget(parent)
{
    valueFont.setPixelSize(valueNameSize);
    QFontMetrics fontMetrics(valueFont);
    valueFontHeight = fontMetrics.height();
    setFixedWidth(calcWidth());
    setFixedHeight(2*valueFontHeight);
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

QList<int> *WaveformShaper::getValues() {
    return values;
}

void WaveformShaper::setValues(QList<int> *newValues) {
    values = newValues;
    updateSize();
}

/*************************************************************************/

void WaveformShaper::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    const int envelopeLength = pPercussion->getEnvelopeLength();

    // Left side
    painter.fillRect(0, 0, legendCellSize, 2*valueFontHeight, MainWindow::lightHighlighted);
    // Bottom
    painter.fillRect(legendCellSize, valueFontHeight, envelopeLength*cellWidth, valueFontHeight, MainWindow::lightHighlighted);

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

