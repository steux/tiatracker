#ifndef INSTRUMENTSELECTOR_H
#define INSTRUMENTSELECTOR_H

#include <QObject>
#include <QWidget>
#include <QFont>
#include "track/track.h"
#include "pianokeyboard.h"


class InstrumentSelector : public QWidget
{
    Q_OBJECT
public:
    explicit InstrumentSelector(QWidget *parent = 0);

    void registerTrack(Track::Track *newTrack);
    void registerPianoKeyboard(PianoKeyboard *pNewKeyboard);

    QSize sizeHint() const;

signals:
    void setWaveform(TiaSound::Distortion dist);
    void setUsePitchGuide(bool use);

public slots:

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    static const int horizontalMargin = 10;
    static const int verticalMargin = 10;
    static const int buttonHorizontalMargin = 6;
    static const int buttonVerticalMargin = 1;
    static const int insPercMargin = 8;
    static const int fontSize = 12;
    static const int minWidth = 176;


    Track::Track *pTrack = nullptr;
    PianoKeyboard *pKeyboard = nullptr;

    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    QFont font{"Helvetica"};
    int widgetHeight;
    int fontHeight;
    int buttonWidth;
    int buttonHeight;

    int selected = 0;
};

#endif // INSTRUMENTSELECTOR_H
