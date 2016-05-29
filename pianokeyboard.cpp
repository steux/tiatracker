/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

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

PianoKeyboard::~PianoKeyboard()
{
    for (int i = 0; i < shortcutActions.size(); ++i) {
        delete shortcutActions[i];
    }
}

/*************************************************************************/

void PianoKeyboard::initPianoKeyboard() {
    // Octave shortcuts
    addShortcut(&actionOctaveUp, "OctaveUp");
    QObject::connect(&actionOctaveUp, SIGNAL(triggered(bool)), this, SLOT(octaveUp(bool)));
    addShortcut(&actionOctaveDown, "OctaveDown");
    QObject::connect(&actionOctaveDown, SIGNAL(triggered(bool)), this, SLOT(octaveDown(bool)));
    for (int i = 0; i < 6; ++i) {
        QString actionName = "Octave" + QString::number(i + 1);
        QString shortcut = MainWindow::keymap[actionName].toString();
        QAction *action = new QAction(this);
        action->setShortcut(QKeySequence(shortcut));
        QObject::connect(action, SIGNAL(triggered(bool)), this, SLOT(changeOctave(bool)));
        action->setData(QVariant(i));
        addAction(action);
        shortcutActions.append(action);
    }

    // Note shortcuts
    for (int octave = 0; octave < 3; ++octave) {
        for (int i = 0; i < 12; ++i) {
            TiaSound::Note n = TiaSound::Note((static_cast<TiaSound::Note>(octave*12 + i)));
            QString noteName = TiaSound::getNoteNameWithOctaveFixedWidth(n);
            QString actionName = "Note" + noteName;
            QString shortcut = MainWindow::keymap[actionName].toString();
            QAction *action = new QAction(this);
            action->setShortcut(QKeySequence(shortcut));
            QObject::connect(action, SIGNAL(triggered(bool)), this, SLOT(pianoKeyShortcut(bool)));
            action->setData(QVariant(octave*12 + i));
            addAction(action);
            shortcutActions.append(action);
        }
    }

}

/*************************************************************************/

void PianoKeyboard::setInstrumentPitchGuide(TiaSound::InstrumentPitchGuide *pitchGuide) {
    for (int i = 0; i < numKeys; ++i) {
        keyInfo[i].isEnabled = false;
    }
    int smallestOctave = 99;
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
                    if (int(iNote/12) < smallestOctave) {
                        smallestOctave = int(iNote/12);
                    }
                }
            }
        }
    }
    setOctave(smallestOctave);
    setUsePitchGuide(true);
}

/*************************************************************************/

void PianoKeyboard::setUsePitchGuide(bool value) {
    usePitchGuide = value;
    update();
}

/*************************************************************************/

void PianoKeyboard::setOctave(int newOctave) {
    if (newOctave >= 0 && newOctave <= 5) {
        selectedOctave = newOctave;
        update();
    }
}

/*************************************************************************/

void PianoKeyboard::octaveUp(bool) {
    setOctave(selectedOctave + 1);
}

/*************************************************************************/

void PianoKeyboard::octaveDown(bool) {
    setOctave(selectedOctave - 1);
}

/*************************************************************************/

void PianoKeyboard::changeOctave(bool) {
    QAction *action = qobject_cast<QAction *>(sender());
    int newOctave = action->data().toInt();
    setOctave(newOctave);
}

/*************************************************************************/

void PianoKeyboard::pianoKeyShortcut(bool) {
    QAction *action = qobject_cast<QAction *>(sender());
    int noteBaseIndex = action->data().toInt();
    int keyIndex = selectedOctave*12 + noteBaseIndex;
    if (keyIndex < numKeys) {
        if (usePitchGuide) {
            if (keyInfo[keyIndex].isEnabled) {
                emit newKeyPressed(keyInfo[keyIndex].frequency);
                update();
            }
        } else {
            emit newKeyPressed(-1);
            update();
        }
    }
}

/*************************************************************************/

void PianoKeyboard::setOffThreshold(int value) {
    offThreshold = value;
    update();
}

/*************************************************************************/

void PianoKeyboard::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    // White keys
    painter.setPen(MainWindow::dark);
    for (int key = 0; key < numKeys; ++key) {
        if (!octaveTraits[key%numKeysPerOctave].isBlack) {
            const int xPos = calcWhiteKeyXPos(key);
            if (!usePitchGuide || keyInfo[key].isEnabled) {
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
    painter.setFont(keyFont);
    for (int octave = 0; octave < numOctaves; ++octave) {
        const int xPos = octave*numWhiteKeysPerOctave*keyWidth - 2;
        const int yPos = keyHeight/2 - 4*keyFontHeight;
        if (octave >= selectedOctave && octave <= selectedOctave + 2) {
            painter.setPen(MainWindow::red);
            keyFont.setBold(true);
            painter.setFont(keyFont);
        } else {
            painter.setPen(MainWindow::contentDark);
            keyFont.setBold(false);
            painter.setFont(keyFont);
        }
        painter.drawText(xPos, yPos, keyWidth, keyFontHeight, Qt::AlignHCenter, QString::number(octave + 1));
    }
    keyFont.setBold(false);
    painter.setFont(keyFont);

    // Black keys
    for (int key = 0; key < numKeys; ++key) {
        if (octaveTraits[key%numKeysPerOctave].isBlack) {
            const int xPos = calcBlackKeyXPos(key);
            if (!usePitchGuide || keyInfo[key].isEnabled) {
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
    if (usePitchGuide) {
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

/*************************************************************************/

void PianoKeyboard::addShortcut(QAction *action, QString actionName) {
    if (MainWindow::keymap.contains(actionName)) {
        QString shortcut = MainWindow::keymap[actionName].toString();
        action->setShortcut(QKeySequence(shortcut));
        addAction(action);
    }
}

/*************************************************************************/

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
    if (usePitchGuide) {
        if (keyInfo[keyIndex].isEnabled) {
            isValidKeyPressed = true;
            keyPressed = keyIndex;
            emit newKeyPressed(keyInfo[keyIndex].frequency);
            update();
        }
    } else {
        isValidKeyPressed = true;
        keyPressed = keyIndex;
        emit newKeyPressed(-1);
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
