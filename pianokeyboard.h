#ifndef PIANOKEYBOARD_H
#define PIANOKEYBOARD_H

#include <QWidget>

#include "tiasound/tiasound.h"
#include "tiasound/instrumentpitchguide.h"


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

    // Threshold for unacceptable off-tune percentage
    int offThreshold = 10;

    explicit PianoKeyboard(QWidget *parent = 0);

    /* Set current instrument pitch guide
     */
    void setInstrumentPitchGuide(TiaSound::InstrumentPitchGuide pitchGuide);

signals:

public slots:


protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

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
    static const int keyFontSize = 9;
    static const int keyFontPadding = 1;    // Top/Bottom padding for key hint lines

    QFont keyFont{"Helvetica"};
    int keyFontHeight;
    int keyInfoRectHeight;

    /* Calc x-pos for a given white or black key (0..numKeys)
     */
    int calcWhiteKeyXPos(int key);
    int calcBlackKeyXPos(int key);
};

#endif // PIANOKEYBOARD_H
