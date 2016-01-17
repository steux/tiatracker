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

    QSize sizeHint() const;

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    static const int channelWidth = 80;
    static const int channelMargin = 8;
    static const int channelGap = 8;
    static const int minHeight = 400;

    int widgetWidth;

    Track::Track *pTrack = nullptr;
};

#endif // TIMELINE_H
