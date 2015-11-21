#include "waveformshaper.h"
#include <QPainter>
#include "mainwindow.h"

#include <iostream>

WaveformShaper::WaveformShaper(QWidget *parent) : QWidget(parent)
{
    setFixedWidth(calcWidth());
    setFixedHeight(widgetHeight);

}

void WaveformShaper::setScale(int min, int max)
{
    scaleMin = min;
    scaleMax = max;
    cellHeight = int( (widgetHeight - legendCellSize)/(max - min + 1) );
}



void WaveformShaper::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setPen(MainWindow::dark);
    painter.fillRect(0, 0, width(), height(), MainWindow::dark);
}

QList<int> WaveformShaper::getValues() const
{
    return values;
}

void WaveformShaper::setValues(const QList<int> &value)
{
    values = value;
    setFixedWidth(calcWidth());
}

int WaveformShaper::getReleaseStart() const
{
    return releaseStart;
}

void WaveformShaper::setReleaseStart(int value)
{
    releaseStart = value;
}

int WaveformShaper::getSustainStart() const
{
    return sustainStart;
}

void WaveformShaper::setSustainStart(int value)
{
    sustainStart = value;
}

