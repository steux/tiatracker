#ifndef PIANOKEYBOARD_H
#define PIANOKEYBOARD_H

#include <QWidget>

class PianoKeyboard : public QWidget
{
    Q_OBJECT
public:
    static const int numOctaves = 7;
    static const int numWhiteKeys = numOctaves*7;
    static const int numBlackKeys = numOctaves*5;
    static const int keyWidth = 20;
    static const int keyHeight = 100;
    static const int blackKeyWidth = keyWidth-4;
    static const int blackKeyHeight = int(0.6*keyHeight);
    static const int keyboardWidth = keyHeight*numWhiteKeys;
    static const int keyboardHeight = keyHeight;

    explicit PianoKeyboard(QWidget *parent = 0);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    static const int blackPositions[];
};

#endif // PIANOKEYBOARD_H
