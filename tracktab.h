#ifndef TRACKTAB_H
#define TRACKTAB_H

#include <QObject>
#include <QWidget>
#include "track/track.h"

class TrackTab : public QWidget
{
    Q_OBJECT
public:
    explicit TrackTab(QWidget *parent = 0);

    void registerTrack(Track::Track *newTrack);

    /* Initializes the GUI components. Must be called once during init. */
    void initTrackTab();

    /* Fills GUI elements with data from the track. Called upon changes. */
    void updateTrackTab();

signals:

public slots:

private:
    Track::Track *pTrack = nullptr;

};

#endif // TRACKTAB_H
