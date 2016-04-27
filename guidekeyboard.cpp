/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

#include <QPainter>
#include <QMouseEvent>
#include <iostream>

#include "guidekeyboard.h"
#include "mainwindow.h"


// Fixed key traits (black yes/no, position index)
const GuideKeyboard::KeyGfxTrait GuideKeyboard::octaveTraits[] = {
    {false, 0},     // C
    {true, 1},      // Cis
    {false, 1},     // D
    {true, 2},      // Dis
    {false, 2},     // E
    {false, 3},     // F
    {true, 4},      // Fis
    {false, 4},     // G
    {true, 5},      // Gis
    {false, 5},     // A
    {true, 6},      // Ais
    {false, 6}      // H
};

GuideKeyboard::GuideKeyboard(QWidget *parent) : QWidget(parent)
{
    keyFont.setPixelSize(keyFontSize);
    QFontMetrics fontMetrics(keyFont);
    keyFontHeight = fontMetrics.height();
    keyInfoRectHeight = keyFontHeight;

    setFixedWidth(keyboardWidth+1);
    setFixedHeight(keyboardHeight+1);

    for (int i = 0; i < numKeys; ++i) {
        keyInfo[i].isEnabled = false;
    }
}

/*************************************************************************/

void GuideKeyboard::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    // White keys
    painter.setPen(MainWindow::dark);
    for (int key = 0; key < numKeys; ++key) {
        if (!octaveTraits[key%numKeysPerOctave].isBlack) {
            const int xPos = calcWhiteKeyXPos(key);
            if (keyInfo[key].isEnabled) {
                painter.fillRect(xPos, 0, keyWidth, keyHeight, MainWindow::violet);
            } else {
                painter.fillRect(xPos, 0, keyWidth, keyHeight, MainWindow::light);
            }
            painter.drawRect(xPos, 0, keyWidth, keyHeight);
        }
    }

    // Octave number hints
    painter.setFont(keyFont);
    painter.setPen(MainWindow::contentDark);
    keyFont.setBold(false);
    for (int octave = 0; octave < numOctaves; ++octave) {
        const int xPos = octave*numWhiteKeysPerOctave*keyWidth - 2;
        const int yPos = keyHeight/2 - 4*keyFontHeight;
        painter.drawText(xPos, yPos, keyWidth, keyFontHeight, Qt::AlignHCenter, QString::number(octave + 1));
    }
    keyFont.setBold(false);
    painter.setFont(keyFont);

    // Black keys
    for (int key = 0; key < numKeys; ++key) {
        if (octaveTraits[key%numKeysPerOctave].isBlack) {
            const int xPos = calcBlackKeyXPos(key);
            // First draw black frame, then colored interior
            painter.fillRect(xPos, 0, blackKeyWidth, blackKeyHeight, MainWindow::darkHighlighted);
            if (keyInfo[key].isEnabled) {
                painter.fillRect(xPos + 1, 1, blackKeyWidth - 2, blackKeyHeight - 2, MainWindow::violet);
            } else {
                painter.fillRect(xPos + 1, 1, blackKeyWidth - 2, blackKeyHeight - 2, MainWindow::darkHighlighted);
            }
        }
    }

    // Hints
    painter.setFont(keyFont);
    for (int key = 0; key < numKeys; ++key) {
        int xPos;
        int rectWidth;
        int rectHeight;
        if (octaveTraits[key%numKeysPerOctave].isBlack) {
            painter.setPen(MainWindow::light);
            xPos = calcBlackKeyXPos(key);
            rectWidth = blackKeyWidth;
            rectHeight = blackKeyHeight;
        } else {
            painter.setPen(MainWindow::dark);
            xPos = calcWhiteKeyXPos(key);
            rectWidth = keyWidth;
            rectHeight = keyHeight;
        }
//        if (std::abs(keyInfo[key].off) >= std::abs(offThreshold)) {
//            painter.setPen(MainWindow::red);
//        }
//        painter.drawText(xPos, rectHeight - 3*keyInfoRectHeight, rectWidth, keyInfoRectHeight, Qt::AlignHCenter|Qt::AlignBottom, QString::number(keyInfo[key].frequency));
//        painter.drawText(xPos, rectHeight - 2*keyInfoRectHeight, rectWidth, keyInfoRectHeight, Qt::AlignHCenter|Qt::AlignBottom, QString::number(keyInfo[key].off));
        TiaSound::Note curNote = TiaSound::getNoteFromInt(key);
        painter.drawText(xPos, rectHeight - 1*keyInfoRectHeight, rectWidth, keyInfoRectHeight, Qt::AlignHCenter|Qt::AlignBottom, TiaSound::getNoteName(curNote));
    }
}

/*************************************************************************/

void GuideKeyboard::mousePressEvent(QMouseEvent *event) {
    int octave = int(event->x()/(keyWidth*numWhiteKeysPerOctave));
    int keyIndex;
    if (event->y() < blackKeyHeight) {
        // Potential black key
        // TODO: More intelligent calculation instead of brute force
        int octaveBaseKey = octave*12;
        int xPos = event->x();
        keyIndex = -1;
        for (int key = 0; key < 12; ++key) {
            if (octaveTraits[key].isBlack) {
                int xPosCandidate = calcBlackKeyXPos(octaveBaseKey + key);
                if (xPos >= xPosCandidate && xPos < xPosCandidate + blackKeyWidth) {
                    keyIndex = octaveBaseKey + key;
                }
            }
        }
        if (keyIndex == -1) {
            // Click is between black keys: White key instead
            keyIndex = calcKeyIndexForWhiteKey(xPos);
        }
    } else {
        // White key
        keyIndex = calcKeyIndexForWhiteKey(event->x());
    }

    keyInfo[keyIndex].isEnabled = !keyInfo[keyIndex].isEnabled;
    update();
}

/*************************************************************************/

int GuideKeyboard::calcWhiteKeyXPos(int key) {
    int keyInOctave = key%numKeysPerOctave;
    int octave = int(key/numKeysPerOctave);
    int octaveXPos = octave*keyWidth*numWhiteKeysPerOctave;
    return octaveXPos + octaveTraits[keyInOctave].posIndex*keyWidth;
}

/*************************************************************************/

int GuideKeyboard::calcBlackKeyXPos(int key) {
    int keyInOctave = key%numKeysPerOctave;
    int octave = int(key/numKeysPerOctave);
    int octaveXPos = octave*keyWidth*numWhiteKeysPerOctave;
    return octaveXPos + octaveTraits[keyInOctave].posIndex*keyWidth - blackKeyWidth/2;
}

/*************************************************************************/

int GuideKeyboard::calcKeyIndexForWhiteKey(int xPos) {
    // Maps index of a white key to overall key index including black keys,
    // which for example can be used with octaveTraits[].
    static const int whiteKeyIndexToKeyIndex[] = {0, 2, 4, 5, 7, 9, 11};

    int octave = int(xPos/(keyWidth*numWhiteKeysPerOctave));
    int xPosInOctave = xPos%(keyWidth*numWhiteKeysPerOctave);
    int whiteKeyIndex = int(xPosInOctave/keyWidth);
    int keyIndex = octave*12 + whiteKeyIndexToKeyIndex[whiteKeyIndex];
    return keyIndex;
}

