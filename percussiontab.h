#ifndef PERCUSSIONTAB_H
#define PERCUSSIONTAB_H

#include <QObject>
#include <QWidget>
#include "track/track.h"
#include "track/instrument.h"

class PercussionTab : public QWidget
{
    Q_OBJECT
public:
    static const int maxPercussionNameLength = 64;

    explicit PercussionTab(QWidget *parent = 0);

    void registerTrack(Track::Track *newTrack);

    /* Initializes the GUI components. Must be called once during init. */
    void initPercussionTab();

    /* Fills GUI elements with data from the track. Called upon changes. */
    void updatePercussionTab();

    /* Returns the currently selected percussion from the percussions tab */
    int getSelectedPercussionIndex();

signals:

public slots:

    void on_buttonPercussionDelete_clicked();

protected:
    Track::Percussion * getSelectedPercussion();

private:
    static const QList<TiaSound::Distortion> availableWaveforms;

    Track::Track *pTrack = nullptr;

};

#endif // PERCUSSIONTAB_H
