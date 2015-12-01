#ifndef PIANOKEYBOARD_H
#define PIANOKEYBOARD_H

#include <QWidget>

#include "tiasound/tiasound.h"
#include "tiasound/instrumentpitchguide.h"

/* A graphical representation of a piano keyboard with a pitch guide for
 * available note info and clickable keys.
 */
class PianoKeyboard : public QWidget
{
    Q_OBJECT
public:
    // Statistics about keys
    static const int numOctaves = 7;
    static const int numWhiteKeysPerOctave = 7;
    static const int numKeysPerOctave = 12;
    static const int numWhiteKeys = numOctaves*7;
    static const int numKeys = numOctaves*numKeysPerOctave;
    // GFX constants
    static const int keyWidth = 20;
    static const int keyHeight = 110;
    static const int blackKeyWidth = keyWidth-4;
    static const int blackKeyHeight = int(0.6*keyHeight);
    static const int keyboardWidth = keyWidth*numWhiteKeys;
    static const int keyboardHeight = keyHeight;

    explicit PianoKeyboard(QWidget *parent = 0);

    /* Inits all isEnabled to false, then goes through list in pitchGuide.
     * Keys for found notes are activated and frequency, note and off-tune value
     * are stored. If a note has multiple entries, the one with the least
     * off-tune value is chosen.
     */
    void setInstrumentPitchGuide(TiaSound::InstrumentPitchGuide pitchGuide);

    // Threshold for unacceptable off-tune percentage
    int offThreshold = 10;

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

    // color and pos data for keys of an octave
    struct KeyGfxTrait {
        bool isBlack;
        int posIndex;   // with regards to white or black row
    };
    static const KeyGfxTrait octaveTraits[];

private:
    // Key states and pitch info
    struct KeyInformation {
        int frequency;
        TiaSound::Note note;
        int off;
        bool isEnabled;
    };

    KeyInformation keyInfo[numKeys]{};

    // Key hints
    static const int keyFontSize = 10;

    QFont keyFont{"Helvetica"};
    int keyFontHeight;
    int keyInfoRectHeight;

    /* Calc x-pos for a given white or black key (0..numKeys) */
    int calcWhiteKeyXPos(int key);
    int calcBlackKeyXPos(int key);
};

#endif // PIANOKEYBOARD_H
