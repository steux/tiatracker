#include "waveformshaper.h"
#include <QPainter>
#include "mainwindow.h"

#include <iostream>

WaveformShaper::WaveformShaper(QWidget *parent) : QWidget(parent)
{
    setFixedWidth(200);
    setFixedHeight(200);

}



void WaveformShaper::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setPen(MainWindow::dark);
    painter.fillRect(0, 0, width(), height(), MainWindow::dark);
}

