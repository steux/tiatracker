#ifndef PIANOKEYBOARD_H
#define PIANOKEYBOARD_H

#include <QWidget>

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
    static const int keyHeight = 100;
    static const int blackKeyWidth = keyWidth-4;
    static const int blackKeyHeight = int(0.6*keyHeight);
    static const int keyboardWidth = keyWidth*numWhiteKeys;
    static const int keyboardHeight = keyHeight;

    explicit PianoKeyboard(QWidget *parent = 0);

signals:

public slots:


protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

    struct keyGfxTrait {
        bool isBlack;
        int posIndex;   // with regards to white or black row
    };
    static const keyGfxTrait octaveTraits[];


private:
    // Key state
    QList<bool> isPressed;

    // Key hints
    static const int keyFontSize = 8;
    static const int keyFontPadding = 4;    // Top/Bottom padding for key hint lines

    QFont keyFont{"Helvetica"};
    int keyFontHeight;
};

#endif // PIANOKEYBOARD_H
