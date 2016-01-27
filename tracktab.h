#ifndef TRACKTAB_H
#define TRACKTAB_H

#include <QObject>
#include <QWidget>
#include "track/track.h"
#include "tiasound/pitchguidefactory.h"
#include "tiasound/pitchguide.h"
#include "tiasound/instrumentpitchguide.h"

class TrackTab : public QWidget
{
    Q_OBJECT
public:
    explicit TrackTab(QWidget *parent = 0);

    void registerTrack(Track::Track *newTrack);
    void registerPitchGuide(TiaSound::PitchGuide *newGuide);

    /* Initializes the GUI components. Must be called once during init. */
    void initTrackTab();

    /* Fills GUI elements with data from the track. Called upon changes. */
    void updateTrackTab();

signals:

public slots:

private:
    Track::Track *pTrack = nullptr;
    TiaSound::PitchGuide *pPitchGuide;

};

#endif // TRACKTAB_H
