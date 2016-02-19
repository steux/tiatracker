#ifndef TRACKTAB_H
#define TRACKTAB_H

#include <QObject>
#include <QWidget>
#include "track/track.h"
#include "tiasound/pitchguidefactory.h"
#include "tiasound/pitchguide.h"
#include "tiasound/instrumentpitchguide.h"
#include <QMenu>
#include <QAction>


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
    void setEvenSpeed(int value);
    void setOddSpeed(int value);
    void channelContextEvent(int channel, int noteIndex);

    // Pattern context menu
    void setStartPattern(bool);
    void renamePattern(bool);
    void setGoto(bool);
    void removeGoto(bool);
    void movePatternUp(bool);
    void movePatternDown(bool);

    // Channel context menu
    void setSlideValue(bool);
    void setFrequency(bool);
    void setHold(bool);
    void setPause(bool);


private:
    /* Just updates the number of patterns and track length stats */
    void updateTrackStats();
    /* Updates the pattern editor area */
    void updatePatternEditor();

    Track::Track *pTrack = nullptr;
    TiaSound::PitchGuide *pPitchGuide;

    QMenu patternContextMenu{this};
    QAction actionInsertPatternBefore{"Insert pattern before...", this};
    QAction actionInsertPatternAfter{"Insert pattern after...", this};
    QAction actionMovePatternUp{"Move pattern up", this};
    QAction actionMovePatternDown{"Move pattern down", this};
    QAction actionRemovePattern{"Remove pattern", this};
    QAction actionRenamePattern{"Rename pattern...", this};
    QAction actionSetGoto{"Set goto...", this};
    QAction actionRemoveGoto{"Remove goto", this};
    QAction actionSetStartPattern{"Start track here", this};

    QMenu channelContextMenu{this};
    QAction actionPause{"Set to PAUSE", this};
    QAction actionHold{"Set to HOLD", this};
    QAction actionSlide{"Set to SLIDE...", this};
    QAction actionSetFrequency{"Change frequency...", this};
    QAction actionInsertRowBefore{"Insert row before", this};
    QAction actionInsertRowAfter{"Insert row after", this};
    QAction actionDeleteRow{"Delete row", this};
    QAction actionMuteChannel{"Mute channel", this};

    int contextEventChannel;
    int contextEventNoteIndex;

};

#endif // TRACKTAB_H
