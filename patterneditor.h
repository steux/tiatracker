/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

#ifndef PATTERNEDITOR_H
#define PATTERNEDITOR_H

#include <QObject>
#include <QWidget>
#include "track/track.h"
#include <QFontDatabase>
#include "tiasound/pitchguide.h"
#include <QMenu>
#include "instrumentselector.h"
#include "emulation/player.h"


class PatternEditor : public QWidget
{
    Q_OBJECT
public:
    explicit PatternEditor(QWidget *parent = 0);

    void registerTrack(Track::Track *newTrack);
    void registerPitchGuide(TiaSound::PitchGuide *newGuide);
    void registerPlayer(Emulation::Player *newPlayer);
    void registerMuteAction(QAction *newAction);
    void registerPatternMenu(QMenu *newPatternMenu);
    void registerChannelMenu(QMenu *newChannelMenu);
    void registerInstrumentSelector(InstrumentSelector *selector);

    int getEditPos();
    int getSelectedChannel();

    QString constructRowString(int curPatternNoteIndex, Track::Pattern *curPattern);

    QSize sizeHint() const;

signals:
    void editPosChanged(int newPos);
    void channelContextEvent(int channel, int row);
    void editChannelChanged(int newChannel);

public slots:
    // Global actions
    void moveUp(bool);
    void moveDown(bool);
    void moveLeft(bool);
    void moveRight(bool);
    void switchChannel(bool);
    void gotoFirstRow(bool);
    void gotoLastRow(bool);
    void gotoNextPattern(bool);
    void gotoPreviousPattern(bool);

    void setEditPos(int newPos);
    void setEditPos(int newChannel, int newPos);

    void validateEditPos();
    void advanceEditPos();
    void setRowsPerBeat(int value);

    void setRowToInstrument(int frequency);

    void newPlayerPos(int pos1, int pos2);

    // Slot for QCheckBox "follow"
    void toggleFollow_clicked(bool toggle);

    void toggleLoop_clicked(bool toggle);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;

private:
    static const int noteFontSize = 12;
    static const int legendFontSize = 12;

    static const int noteMargin = 6;
    static const int patternNameWidth = 120;
    static const int patternNameMargin = 4;
    static const int minHeight = 400;

    void drawPatternNameAndSeparator(int yPos, int nameXPos, int curPatternNoteIndex, int channel, int xPos, int curEntryIndex, QPainter *painter, Track::Pattern *curPattern);
    void drawGoto(int channel, int yPos, Track::Pattern *curPattern, Track::SequenceEntry *curEntry, QPainter *painter, int nameXPos, int curPatternNoteIndex);
    void drawTimestamp(int row, QPainter *painter, int yPos, int channel);
    void paintChannel(QPainter *painter, int channel, int xPos, int nameXPos);

    /* If x and y are in a valid row with regards to the channel clicked,
     * the channel number, row and note index get written to the parameters
     * and true is returned.
     */
    bool clickedInValidRow(int x, int y, int *channel, int *noteIndex);

    int calcChannelRowPos(int yPos);

    int numRows;    // number of visible rows
    int topMargin;  // margin before first row is displayed

    int noteFontHeight;
    int noteAreaWidth;
    int timeAreaWidth;
    int widgetWidth;

    QFont noteFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    QFont legendFont{"Helvetica"};
    int legendFontHeight;
    int timelineWidth;

    Track::Track *pTrack;
    TiaSound::PitchGuide *pPitchGuide;
    Emulation::Player *pPlayer = nullptr;
    QAction *muteAction = nullptr;
    QMenu *pPatternMenu = nullptr;
    QMenu *pChannelMenu = nullptr;
    InstrumentSelector *pInsSelector = nullptr;

    int selectedChannel = 0;
    // Current editor note focus, i.e. middle-of-screen highlight
    int editPos = 0;

    bool follow = false;
    bool loop = false;
};

#endif // PATTERNEDITOR_H
