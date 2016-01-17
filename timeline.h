#ifndef TIMELINE_H
#define TIMELINE_H

#include <QObject>
#include <QWidget>
#include "track/track.h"

class Timeline : public QWidget
{
    Q_OBJECT
public:
    explicit Timeline(QWidget *parent = 0);

    void registerTrack(Track::Track *newTrack);

signals:

public slots:

private:
    static const int channelWidth = 50;
    static const int channelMargin = 8;

    Track::Track *pTrack = nullptr;
};

#endif // TIMELINE_H
