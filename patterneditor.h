#ifndef PATTERNEDITOR_H
#define PATTERNEDITOR_H

#include <QObject>
#include <QWidget>
#include "track/track.h"
#include <QFontDatabase>


class PatternEditor : public QWidget
{
    Q_OBJECT
public:
    explicit PatternEditor(QWidget *parent = 0);

    void registerTrack(Track::Track *newTrack);

    QSize sizeHint() const;

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    static const int noteFontSize = 12;
    static const int legendFontSize = 12;

    static const int noteMargin = 6;
    static const int patternNameWidth = 120;
    static const int patternNameMargin = 4;
    static const int minHeight = 400;

    void paintChannel(QPainter *painter, int channel, int xPos, int yOffset, int numRows, int nameXPos);

    int noteFontHeight;
    int noteAreaWidth;
    int timeAreaWidth;
    int widgetWidth;

    QFont noteFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    QFont legendFont{"Helvetica"};
    int legendFontHeight;
    int timelineWidth;

    Track::Track *pTrack;

    // Current editor note focus, i.e. middle-of-screen highlight
    int editPos = 0;
};

#endif // PATTERNEDITOR_H
