#include "pianokeyboard.h"
#include <QPainter>

const int PianoKeyboard::blackPositions[] = {1, 2, 4, 5, 6};



PianoKeyboard::PianoKeyboard(QWidget *parent) : QWidget(parent)
{

}



void PianoKeyboard::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::black);
    // White keys
    for (int i = 0; i < numWhiteKeys; ++i) {
        painter.fillRect(i*keyWidth, 0, keyWidth, keyHeight, Qt::white);
        painter.drawRect(i*keyWidth, 0, keyWidth, keyHeight);
    }
    // Black keys
    for (int octave = 0; octave < numOctaves; ++octave) {
        for (int i  = 0; i < 5; ++i) {
            const int xPos = (octave*7+blackPositions[i])*keyWidth - blackKeyWidth/2;
            painter.fillRect(xPos, 0, blackKeyWidth, blackKeyHeight, Qt::black);
        }
    }

    QFont font("Helvetica");
    font.setPixelSize(8);
    painter.setFont(font);

    painter.setPen(Qt::white);
    painter.drawText(keyWidth - blackKeyWidth/2, 0, blackKeyWidth, blackKeyHeight, Qt::AlignBottom|Qt::AlignHCenter, "-37");

}
