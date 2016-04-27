/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

#ifndef GUIDEKEYBOARD_H
#define GUIDEKEYBOARD_H

#include <QObject>
#include <QWidget>

#include "tiasound/tiasound.h"
#include "tiasound/instrumentpitchguide.h"

class GuideKeyboard : public QWidget
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

    explicit GuideKeyboard(QWidget *parent = 0);

    void setInstrumentPitchGuide(TiaSound::InstrumentPitchGuide *pitchGuide);

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

    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    // Key states and pitch info
    struct KeyInformation {
        int frequency;
        TiaSound::Note note;
        int off;
        bool isEnabled;
    };

    KeyInformation keyInfo[numKeys]{};

    // If a valid key is pressed with the mouse
    bool isValidKeyPressed = false;
    // Index of key that is currently pressed
    int keyPressed = -1;

    // Key hints
    static const int keyFontSize = 10;

    QFont keyFont{"Helvetica"};
    int keyFontHeight;
    int keyInfoRectHeight;

    bool usePitchGuide = false;

    /* Calc x-pos for a given white or black key (0..numKeys) */
    int calcWhiteKeyXPos(int key);
    int calcBlackKeyXPos(int key);

    /* Calc the key index for a white key for a given x coordinate */
    int calcKeyIndexForWhiteKey(int xPos);
};

#endif // GUIDEKEYBOARD_H
