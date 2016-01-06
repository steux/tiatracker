#include "PercussionShaper.h"
#include <QPainter>
#include "mainwindow.h"
#include <cassert>
#include <QMouseEvent>

#include <iostream>


PercussionShaper::PercussionShaper(QWidget *parent) : QWidget(parent)
{
    setFixedWidth(calcWidth());
    setFixedHeight(widgetHeight);
    legendFont.setPixelSize(legendNameSize);
    QFontMetrics fontMetrics(legendFont);
    nameFontHeight = fontMetrics.height();
}

/*************************************************************************/

void PercussionShaper::registerPercussion(Track::Percussion *newPercussion) {
     pPercussion = newPercussion;
}

/*************************************************************************/

void PercussionShaper::setScale(int min, int max) {
    scaleMin = min;
    scaleMax = max;
    cellHeight = int( (widgetHeight - legendCellSize)/(max - min + 1) );
}

/*************************************************************************/

void PercussionShaper::drawLegend(QPainter &painter,
                                const int valuesXPos, const int valuesHeight) {
    int envelopeLength = pPercussion->getEnvelopeLength();

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
    int min = scaleMin;
    int max = scaleMax;
    if (isInverted) {
        min = scaleMax;
        max = scaleMin;
    }
    painter.drawText(0, 0, legendCellSize, nameFontHeight, Qt::AlignCenter, QString::number(max));
    painter.drawText(0, valuesHeight - nameFontHeight, legendCellSize, nameFontHeight, Qt::AlignCenter, QString::number(min));
    // Frame numbers
    for (int frame = 0; frame < envelopeLength; ++frame) {
        int xPos = legendCellSize + frame*cellWidth;
        painter.drawText(xPos, valuesHeight, cellWidth, legendCellSize, Qt::AlignCenter, QString::number(frame + 1));
    }
}

void PercussionShaper::drawWaveform(QPainter &painter, const int valuesXPos) {
    int envelopeLength = pPercussion->getEnvelopeLength();

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
        if (isInverted) {
            deviceValue = scaleMax - scaleMin - deviceValue;
        }
        int yPos = int(deviceValue*cellHeight + cellHeight/2);
        painter.drawEllipse(xPos - valueCircleRadius, yPos - valueCircleRadius, 2*valueCircleRadius - 1, 2*valueCircleRadius - 1);
    }
    // Value lines
    painter.setPen(QPen(MainWindow::blue, 2, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
    for (int i = 1; i < envelopeLength; ++i) {
        int fromX = int(valuesXPos + (i - 1)*cellWidth + cellWidth/2);
        int deviceValueFrom = scaleMax - (*values)[i - 1];
        if (isInverted) {
            deviceValueFrom = scaleMax - scaleMin - deviceValueFrom;
        }
        int fromY = int(deviceValueFrom*cellHeight + cellHeight/2);
        int toX = int(valuesXPos + i*cellWidth + cellWidth/2);
        int deviceValueTo = scaleMax - (*values)[i];
        if (isInverted) {
            deviceValueTo = scaleMax - scaleMin - deviceValueTo;
        }
        int toY = int(deviceValueTo*cellHeight + cellHeight/2);
        painter.drawLine(fromX, fromY, toX, toY);
    }
}

void PercussionShaper::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    const int valuesXPos = legendCellSize;
    const int valuesHeight = widgetHeight - legendCellSize;

    drawLegend(painter, valuesXPos, valuesHeight);

    // Background for values
    int envelopeLength = pPercussion->getEnvelopeLength();
    painter.fillRect(valuesXPos, 0, envelopeLength*cellWidth, valuesHeight, MainWindow::dark);

    /* Waveform */
    drawWaveform(painter, valuesXPos);
}

/*************************************************************************/

void PercussionShaper::processMouseEvent(int x, int y) {
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
        if (iValue >=0 && iValue < pPercussion->getEnvelopeLength()
                && newValue >= scaleMin && newValue <= scaleMax) {
            if (isInverted) {
                newValue = scaleMin + scaleMax - newValue;
            }
            bool isNew = false;
            if ((*values)[iValue] != newValue) {
                isNew = true;
            }
            (*values)[iValue] = newValue;
            draggingIndex = iValue;
            if (isNew) {
                emit newPercussionValue(iValue);
            }
            update();
        }
    }
}

void PercussionShaper::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isMouseDragging = true;
        processMouseEvent(event->x(), event->y());
    }
}

void PercussionShaper::mouseReleaseEvent(QMouseEvent *) {
    isMouseDragging = false;
    draggingIndex = -1;
    int newMax = pPercussion->getMaxVolume();
    emit newMaxValue(newMax);
    emit silence();
}

void PercussionShaper::mouseMoveEvent(QMouseEvent *event) {
    if (isMouseDragging) {
        processMouseEvent(event->x(), event->y());
    }
}

/*************************************************************************/

void PercussionShaper::contextMenuEvent(QContextMenuEvent *event) {
    int valueAreaHeight = widgetHeight - legendCellSize;
    if (event->x() >= legendCellSize && event->y() < valueAreaHeight) {
        int frame = (event->x() - legendCellSize)/cellWidth;
        emit envelopeContextEvent(frame);
        contextMenu->exec(event->globalPos());
    }
}

/*************************************************************************/

int PercussionShaper::calcWidth() {
    int envelopeLength = 1;
    // During init, no instrument is registered yet
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

void PercussionShaper::updateSize() {
    setFixedWidth(calcWidth());
}

/*************************************************************************/

QList<int>* PercussionShaper::getValues() {
    return values;
}

void PercussionShaper::setValues(QList<int> *newValues) {
    values = newValues;
    updateSize();
}
