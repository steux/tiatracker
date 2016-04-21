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

signals:

public slots:
};

#endif // GUIDEKEYBOARD_H
