#include "pianokeyboard.h"
#include <QPainter>

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



PianoKeyboard::PianoKeyboard(QWidget *parent) : QWidget(parent)
{
    keyFont.setPixelSize(keyFontSize);
    QFontMetrics fontMetrics(keyFont);
    keyFontHeight = fontMetrics.height();

    setFixedWidth(keyboardWidth+1);
    setFixedHeight(keyboardHeight+1);
}



/* Inits all isEnabled to false, then goes through list in pitchGuide.
 * Keys for found notes are activated and frequeny, note and off-tune value
 * are stored. If a note has multiple entries, the one with the least
 * off-tune value is chosen.
 */
void PianoKeyboard::setInstrumentPitchGuide(TiaSound::InstrumentPitchGuide pitchGuide)
{
    for (int i = 0; i < numKeys; ++i) {
        keyInfo[i].isEnabled = false;
    }
    for (int freq = 0; freq < pitchGuide.getNumFrequencies(); ++freq) {
        TiaSound::Note note = pitchGuide.getNote(freq);
        int iNote = static_cast<int>(note);
        int off = pitchGuide.getPercentOff(freq);
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



/* Paint keyboard
 */
void PianoKeyboard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::black);

    // White keys
    for (int key = 0; key < numKeys; ++key) {
        if (!octaveTraits[key%numKeysPerOctave].isBlack) {
            const int xPos = calcWhiteKeyXPos(key);
            painter.fillRect(xPos, 0, keyWidth, keyHeight, Qt::white);
            painter.drawRect(xPos, 0, keyWidth, keyHeight);
        }
    }

    // Black keys
    for (int key = 0; key < numKeys; ++key) {
        if (octaveTraits[key%numKeysPerOctave].isBlack) {
            const int xPos = calcBlackKeyXPos(key);
            painter.fillRect(xPos, 0, blackKeyWidth, blackKeyHeight, Qt::black);
        }
    }

    // Hints
    painter.setFont(keyFont);
//    painter.setPen(Qt::white);
//    painter.drawText(keyWidth - blackKeyWidth/2, 0, blackKeyWidth, blackKeyHeight, Qt::AlignBottom|Qt::AlignHCenter, "-37");

    for (int key = 0; key < numKeys; ++key) {
        if (keyInfo[key].isEnabled) {
            int xPos;
            int rectWidth;
            int rectHeight;
            if (octaveTraits[key%numKeysPerOctave].isBlack) {
                painter.setPen(Qt::white);
                xPos = calcBlackKeyXPos(key);
                rectWidth = blackKeyWidth;
                rectHeight = blackKeyHeight;
            } else {
                painter.setPen(Qt::black);
                xPos = calcWhiteKeyXPos(key);
                rectWidth = keyWidth;
                rectHeight = keyHeight;
            }
            painter.drawText(xPos, 0, rectWidth, rectHeight, Qt::AlignHCenter, TiaSound::getNoteName(keyInfo[key].note));
        }
    }

}



/* Calc x-pos for a given white key (0..numKeys)
 */
int PianoKeyboard::calcWhiteKeyXPos(int key)
{
    int keyInOctave = key%numKeysPerOctave;
    int octave = int(key/numKeysPerOctave);
    int octaveXPos = octave*keyWidth*numWhiteKeysPerOctave;
    return octaveXPos + octaveTraits[keyInOctave].posIndex*keyWidth;
}



/* Calc x-pos for a given black key (0..numKeys)
 */
int PianoKeyboard::calcBlackKeyXPos(int key)
{
    int keyInOctave = key%numKeysPerOctave;
    int octave = int(key/numKeysPerOctave);
    int octaveXPos = octave*keyWidth*numWhiteKeysPerOctave;
    return octaveXPos + octaveTraits[keyInOctave].posIndex*keyWidth - blackKeyWidth/2;
}
