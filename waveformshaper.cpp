#include "waveformshaper.h"
#include <QPainter>
#include "mainwindow.h"
#include <cassert>
#include <QMouseEvent>

#include <iostream>


/**************************************************************************
 * Constructor
 *************************************************************************/
WaveformShaper::WaveformShaper(QWidget *parent) : QWidget(parent)
{
    setFixedWidth(calcWidth());
    setFixedHeight(widgetHeight);
    legendFont.setPixelSize(legendNameSize);
    QFontMetrics fontMetrics(legendFont);
    nameFontHeight = fontMetrics.height();
}



/**************************************************************************
 * Register the track, for data retrieval.
 *************************************************************************/
void WaveformShaper::registerInstrument(Track::Instrument *newInstrument)
{
     pInstrument = newInstrument;
}



/**************************************************************************
 * Set min and max for scale
 *************************************************************************/
void WaveformShaper::setScale(int min, int max)
{
    scaleMin = min;
    scaleMax = max;
    cellHeight = int( (widgetHeight - legendCellSize)/(max - min + 1) );
}



/**************************************************************************
 * Paint event with helper functions
 *************************************************************************/
void WaveformShaper::drawLegend(QPainter &painter, const int valuesXPos, const int valuesHeight)
{
    int envelopeLength = pInstrument->getEnvelopeLength();

    // Left side
    painter.fillRect(0, 0, legendCellSize, widgetHeight, MainWindow::lightHighlighted);
    // Bottom
    painter.fillRect(valuesXPos, widgetHeight - legendCellSize, envelopeLength*cellWidth, legendCellSize, MainWindow::lightHighlighted);
    // Name
    painter.setPen(MainWindow::contentDarker);
    legendFont.setBold(false);
    legendFont.setPixelSize(legendNameSize);
    painter.setFont(legendFont);
    painter.save();
    painter.rotate(-90);
    int nameLength = painter.fontMetrics().width(name);
    painter.drawText(0-(valuesHeight - nameLength)/2 - nameLength, 16, name);
    painter.restore();
    // Scale
    legendFont.setBold(false);
    legendFont.setPixelSize(legendScaleSize);
    painter.setFont(legendFont);
    painter.drawText(0, 0, legendCellSize, nameFontHeight, Qt::AlignCenter, QString::number(scaleMax));
    painter.drawText(0, valuesHeight - nameFontHeight, legendCellSize, nameFontHeight, Qt::AlignCenter, QString::number(scaleMin));
    // Frame numbers
    for (int frame = 0; frame < envelopeLength; ++frame) {
        int xPos = legendCellSize + frame*cellWidth;
        painter.drawText(xPos, valuesHeight, cellWidth, legendCellSize, Qt::AlignCenter, QString::number(frame + 1));
    }
}


void WaveformShaper::drawAttackDecay(QPainter &painter, const int valuesXPos, const int valuesHeight)
{
    assert(pInstrument != nullptr);

    int envelopeLength = pInstrument->getEnvelopeLength();
    int sustainStart = pInstrument->getSustainStart();
    int releaseStart = pInstrument->getReleaseStart();

    // Attack/Decay
    painter.fillRect(valuesXPos, 0, sustainStart*cellWidth, valuesHeight, MainWindow::dark);
    // Sustain
    painter.fillRect(valuesXPos + sustainStart*cellWidth, 0, (releaseStart - sustainStart)*cellWidth, valuesHeight, MainWindow::darkHighlighted);
    // Release
    painter.fillRect(valuesXPos + releaseStart*cellWidth, 0, (envelopeLength - releaseStart)*cellWidth, valuesHeight, MainWindow::dark);
}


void WaveformShaper::drawWaveform(QPainter &painter, const int valuesXPos)
{
    int envelopeLength = pInstrument->getEnvelopeLength();

    // Value numbers
    painter.setPen(MainWindow::contentLight);
    for (int iValue = 0; iValue < envelopeLength; ++iValue) {
        int value = (*values)[iValue];
        painter.drawText(valuesXPos + iValue*cellWidth, 0, cellWidth, legendCellSize, Qt::AlignCenter, QString::number(value));
    }
    // Value circles
    painter.setPen(MainWindow::contentLight);
    for (int i = 0; i < envelopeLength; ++i) {
        int xPos = int(valuesXPos + i*cellWidth + cellWidth/2);
        int deviceValue = scaleMax - (*values)[i];
        int yPos = int(deviceValue*cellHeight + cellHeight/2);
        painter.drawEllipse(xPos - valueCircleRadius, yPos - valueCircleRadius, 2*valueCircleRadius - 1, 2*valueCircleRadius - 1);
    }
    // Value lines
    painter.setPen(QPen(MainWindow::blue, 2, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
    for (int i = 1; i < envelopeLength; ++i) {
        int fromX = int(valuesXPos + (i - 1)*cellWidth + cellWidth/2);
        int deviceValueFrom = scaleMax - (*values)[i - 1];
        int fromY = int(deviceValueFrom*cellHeight + cellHeight/2);
        int toX = int(valuesXPos + i*cellWidth + cellWidth/2);
        int deviceValueTo = scaleMax - (*values)[i];
        int toY = int(deviceValueTo*cellHeight + cellHeight/2);
        painter.drawLine(fromX, fromY, toX, toY);
    }
}


void WaveformShaper::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    const int valuesXPos = legendCellSize;
    const int valuesHeight = widgetHeight - legendCellSize;

    drawLegend(painter, valuesXPos, valuesHeight);
    drawAttackDecay(painter, valuesXPos, valuesHeight);

    /* Waveform */
    drawWaveform(painter, valuesXPos);
}

void WaveformShaper::processMouseEvent(int x, int y)
{
    x -= legendCellSize;
    if (x >= 0 && y >= 0) {
        // Click was inside the graph
        int iValue = int(x/cellWidth);
        int newValue = scaleMax - int(y/cellHeight);
        // If we're dragging already, use that index instead
        if (draggingIndex != -1) {
            iValue = draggingIndex;
        }
        // Sanity check in case area is some pixels bigger than graph area
        if (iValue >=0 && iValue < pInstrument->getEnvelopeLength()
                && newValue >= scaleMin && newValue <= scaleMax) {
            (*values)[iValue] = newValue;
            draggingIndex = iValue;
            update();
        }
    }
}


void WaveformShaper::mousePressEvent(QMouseEvent *event)
{
    isMouseDragging = true;
    processMouseEvent(event->x(), event->y());

}

void WaveformShaper::mouseReleaseEvent(QMouseEvent *)
{
    isMouseDragging = false;
    draggingIndex = -1;
    int newMax = pInstrument->getMaxVolume();
    emit newMaxValue(newMax);
}

void WaveformShaper::mouseMoveEvent(QMouseEvent *event)
{
    if (isMouseDragging) {
        processMouseEvent(event->x(), event->y());
    }
}



int WaveformShaper::calcWidth()
{
    int envelopeLength = 2;
    // During init, no instrument is registered yet
    if (pInstrument != nullptr) {
        envelopeLength = pInstrument->getEnvelopeLength();
    }
    int width = legendCellSize;
    if (values != nullptr) {
        width += envelopeLength*cellWidth;
    }
    return width;
}



void WaveformShaper::updateSize()
{
    setFixedWidth(calcWidth());
}



/**************************************************************************
 * Getter/setter
 *************************************************************************/
QList<int>* WaveformShaper::getValues()
{
    return values;
}

void WaveformShaper::setValues(QList<int> *newValues)
{
    values = newValues;
    updateSize();
}

