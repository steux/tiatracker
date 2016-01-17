#ifndef PATTERNEDITOR_H
#define PATTERNEDITOR_H

#include <QObject>
#include <QWidget>
#include "track/track.h"


class PatternEditor : public QWidget
{
    Q_OBJECT
public:
    explicit PatternEditor(QWidget *parent = 0);

    void registerTrack(Track::Track *newTrack);

signals:

public slots:

private:
    static const int trackFontSize = 12;
    static const int legendFontSize = 12;

    int trackFontHeight;
    int trackAreaWidth;

    QFont legendFont{"Helvetica"};
    int legendFontHeight;
    int patternNameWidth;
    int timelineWidth;

    Track::Track *pTrack;
};

#endif // PATTERNEDITOR_H
