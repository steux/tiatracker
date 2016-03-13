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
#include "emulation/player.h"


class TrackTab : public QWidget
{
    Q_OBJECT
public:
    explicit TrackTab(QWidget *parent = 0);

    void registerTrack(Track::Track *newTrack);
    void registerPitchGuide(TiaSound::PitchGuide *newGuide);
    void registerPlayer(Emulation::Player *newPlayer);

    /* Initializes the GUI components. Must be called once during init. */
    void initTrackTab();

    /* Fills GUI elements with data from the track. Called upon changes. */
    void updateTrackTab();

signals:
    void advanceEditPos();
    void validateEditPos();
    void stopTrack();

public slots:
    void setEvenSpeed(int value);
    void setOddSpeed(int value);
    void channelContextEvent(int channel, int noteIndex);
    // For shortcut action "TrackToggleFollow"
    void toggleFollow(bool);
    void toggleLoop(bool);

    // Pattern context menu
    void setStartPattern(bool);
    void renamePattern(bool);
    void setGoto(bool);
    void removeGoto(bool);
    void movePatternUp(bool);
    void movePatternDown(bool);
    void insertPatternBefore(bool);
    void insertPatternAfter(bool);
    void removePattern(bool);
    void duplicatePattern(bool);

    // Channel context menu
    void setSlideValue(bool);
    void setFrequency(bool);
    void setHold(bool);
    void setPause(bool);
    void deleteRow(bool);
    void insertRowBefore(bool);
    void insertRowAfter(bool);
    void toggleMute(bool);

    // Player error
    void invalidNoteFound(int channel, int entryIndex, int noteIndex);

private:
    /* Just updates the number of patterns and track length stats */
    void updateTrackStats();
    /* Updates the pattern editor area */
    void updatePatternEditor();

    /* Lets the user select a pattern to insert. Returns index of
     * Pattern, or patterns.size() if create new was pressed,
     * or -1 if cancel was pressed. */
    int choosePatternToInsert(bool doBefore);

    void addShortcut(QAction *action, QString actionName);

    Track::Track *pTrack = nullptr;
    TiaSound::PitchGuide *pPitchGuide;
    Emulation::Player *pPlayer = nullptr;

    // Global actions
    QAction actionMoveUp{this};
    QAction actionMoveDown{this};
    QAction actionLeftChannel{this};
    QAction actionRightChannel{this};
    QAction actionSwitchChannel{this};
    QAction actionFirstRow{this};
    QAction actionLastRow{this};
    QAction actionNextPattern{this};
    QAction actionPreviousPattern{this};

    // Pattern actions
    QMenu patternContextMenu{this};
    QAction actionInsertPatternBefore{"Insert pattern before...", this};
    QAction actionInsertPatternAfter{"Insert pattern after...", this};
    QAction actionDuplicatePattern{"Duplicate pattern...", this};
    QAction actionMovePatternUp{"Move pattern up", this};
    QAction actionMovePatternDown{"Move pattern down", this};
    QAction actionRemovePattern{"Remove pattern", this};
    QAction actionRenamePattern{"Rename pattern...", this};
    QAction actionSetGoto{"Set goto...", this};
    QAction actionRemoveGoto{"Remove goto", this};
    QAction actionSetStartPattern{"Start track here", this};

    // Channel actions
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

    // Last value the user selected for length of new pattern
    int lastNewPatternLength = 32;
};

#endif // TRACKTAB_H
