#include <QPainter>
#include <QMouseEvent>
#include <iostream>

#include "pianokeyboard.h"
#include "mainwindow.h"


// Fixed key traits (black yes/no, position index)
const PianoKeyboard::KeyGfxTrait PianoKeyboard::octaveTraits[] = {
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

/*************************************************************************/

PianoKeyboard::PianoKeyboard(QWidget *parent) : QWidget(parent)
{
    keyFont.setPixelSize(keyFontSize);
    QFontMetrics fontMetrics(keyFont);
    keyFontHeight = fontMetrics.height();
    keyInfoRectHeight = keyFontHeight;

    setFixedWidth(keyboardWidth+1);
    setFixedHeight(keyboardHeight+1);
}

/*************************************************************************/

void PianoKeyboard::setInstrumentPitchGuide(TiaSound::InstrumentPitchGuide *pitchGuide) {
    for (int i = 0; i < numKeys; ++i) {
        keyInfo[i].isEnabled = false;
    }
    for (int freq = 0; freq < pitchGuide->getNumFrequencies(); ++freq) {
        TiaSound::Note note = pitchGuide->getNote(freq);
        if (note != TiaSound::Note::NotANote) {
            int iNote = static_cast<int>(note);
            if (iNote >= 0 && iNote < numKeys) {
                int off = pitchGuide->getPercentOff(freq);
                if (keyInfo[iNote].isEnabled) {
                    // Duplicate note: Choose note with smaller off-tune value
                    if (off < keyInfo[iNote].off) {
                        keyInfo[iNote].isEnabled = true;
                        keyInfo[iNote].frequency = freq;
                        keyInfo[iNote].note = note;
                        keyInfo[iNote].off = off;
                    }
                } else {
                    // Enable key for this note
                    keyInfo[iNote].isEnabled = true;
                    keyInfo[iNote].frequency = freq;
                    keyInfo[iNote].note = note;
                    keyInfo[iNote].off = off;
                }
            }
        }
    }
}

/*************************************************************************/

void PianoKeyboard::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    // White keys
    painter.setPen(MainWindow::dark);
    for (int key = 0; key < numKeys; ++key) {
        if (!octaveTraits[key%numKeysPerOctave].isBlack) {
            const int xPos = calcWhiteKeyXPos(key);
            if (keyInfo[key].isEnabled) {
                QColor color;
                if (isValidKeyPressed && keyPressed == key) {
                    color = MainWindow::violet;
                } else {
                    color = MainWindow::light;
                }
                painter.fillRect(xPos, 0, keyWidth, keyHeight, color);
            } else {
                painter.fillRect(xPos, 0, keyWidth, keyHeight, MainWindow::lightHighlighted);
            }
            painter.drawRect(xPos, 0, keyWidth, keyHeight);
        }
    }

    // Octave number hints
    painter.setPen(MainWindow::contentDark);
    painter.setFont(keyFont);
    for (int octave = 0; octave < numOctaves; ++octave) {
        const int xPos = octave*numWhiteKeysPerOctave*keyWidth - 2;
        const int yPos = keyHeight/2 - 4*keyFontHeight;
        painter.drawText(xPos, yPos, keyWidth, keyFontHeight, Qt::AlignHCenter, QString::number(octave + 1));
    }

    // Black keys
    for (int key = 0; key < numKeys; ++key) {
        if (octaveTraits[key%numKeysPerOctave].isBlack) {
            const int xPos = calcBlackKeyXPos(key);
            if (keyInfo[key].isEnabled) {
                QColor color;
                if (isValidKeyPressed && keyPressed == key) {
                    color = MainWindow::violet;
                } else {
                    color = MainWindow::dark;
                }
                painter.fillRect(xPos, 0, blackKeyWidth, blackKeyHeight, color);
            } else {
                painter.fillRect(xPos, 0, blackKeyWidth, blackKeyHeight, MainWindow::darkHighlighted);
            }
        }
    }

    // Hints
    painter.setFont(keyFont);
    for (int key = 0; key < numKeys; ++key) {
        if (keyInfo[key].isEnabled) {
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
            if (std::abs(keyInfo[key].off) >= std::abs(offThreshold)) {
                painter.setPen(MainWindow::red);
            }
            painter.drawText(xPos, rectHeight - 3*keyInfoRectHeight, rectWidth, keyInfoRectHeight, Qt::AlignHCenter|Qt::AlignBottom, QString::number(keyInfo[key].frequency));
            painter.drawText(xPos, rectHeight - 2*keyInfoRectHeight, rectWidth, keyInfoRectHeight, Qt::AlignHCenter|Qt::AlignBottom, QString::number(keyInfo[key].off));
            painter.drawText(xPos, rectHeight - 1*keyInfoRectHeight, rectWidth, keyInfoRectHeight, Qt::AlignHCenter|Qt::AlignBottom, TiaSound::getNoteName(keyInfo[key].note));
        }
    }

}

/*************************************************************************/

int PianoKeyboard::calcKeyIndexForWhiteKey(int xPos) {
    // Maps index of a white key to overall key index including black keys,
    // which for example can be used with octaveTraits[].
    static const int whiteKeyIndexToKeyIndex[] = {0, 2, 4, 5, 7, 9, 11};

    int octave = int(xPos/(keyWidth*numWhiteKeysPerOctave));
    int xPosInOctave = xPos%(keyWidth*numWhiteKeysPerOctave);
    int whiteKeyIndex = int(xPosInOctave/keyWidth);
    int keyIndex = octave*12 + whiteKeyIndexToKeyIndex[whiteKeyIndex];
    return keyIndex;
}

void PianoKeyboard::mousePressEvent(QMouseEvent *event)
{
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
    if (keyInfo[keyIndex].isEnabled) {
        isValidKeyPressed = true;
        keyPressed = keyIndex;
        emit newKeyPressed(keyInfo[keyIndex].frequency);
        update();
    }
}

/*************************************************************************/

void PianoKeyboard::mouseReleaseEvent(QMouseEvent *)
{
    if (isValidKeyPressed) {
        isValidKeyPressed = false;
        keyPressed = -1;
        emit keyReleased();
        update();
    }
}

/*************************************************************************/

int PianoKeyboard::calcWhiteKeyXPos(int key) {
    int keyInOctave = key%numKeysPerOctave;
    int octave = int(key/numKeysPerOctave);
    int octaveXPos = octave*keyWidth*numWhiteKeysPerOctave;
    return octaveXPos + octaveTraits[keyInOctave].posIndex*keyWidth;
}

int PianoKeyboard::calcBlackKeyXPos(int key) {
    int keyInOctave = key%numKeysPerOctave;
    int octave = int(key/numKeysPerOctave);
    int octaveXPos = octave*keyWidth*numWhiteKeysPerOctave;
    return octaveXPos + octaveTraits[keyInOctave].posIndex*keyWidth - blackKeyWidth/2;
}
