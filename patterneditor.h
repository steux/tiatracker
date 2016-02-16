#ifndef PATTERNEDITOR_H
#define PATTERNEDITOR_H

#include <QObject>
#include <QWidget>
#include "track/track.h"
#include <QFontDatabase>
#include "tiasound/pitchguide.h"


class PatternEditor : public QWidget
{
    Q_OBJECT
public:
    explicit PatternEditor(QWidget *parent = 0);

    void registerTrack(Track::Track *newTrack);
    void registerPitchGuide(TiaSound::PitchGuide *newGuide);

    int getEditPos();

    QSize sizeHint() const;

signals:
    void editPosChanged(int newPos);

public slots:
    void setEditPos(int newPos);
    void setRowsPerBeat(int value);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *) Q_DECL_OVERRIDE;

private:
    static const int noteFontSize = 12;
    static const int legendFontSize = 12;

    static const int noteMargin = 6;
    static const int patternNameWidth = 120;
    static const int patternNameMargin = 4;
    static const int minHeight = 400;

    QString constructRowString(int curPatternNoteIndex, Track::Pattern *curPattern);
    void drawPatternNameAndSeparator(int yPos, int nameXPos, int curPatternNoteIndex, int channel, int xPos, int curEntryIndex, QPainter *painter, Track::Pattern *curPattern);
    void drawGoto(int channel, int yPos, Track::Pattern *curPattern, Track::SequenceEntry *curEntry, QPainter *painter, int nameXPos, int curPatternNoteIndex);
    void drawTimestamp(int row, QPainter *painter, int yPos, int channel);
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
    TiaSound::PitchGuide *pPitchGuide;

    int selectedChannel = 1;
    // Current editor note focus, i.e. middle-of-screen highlight
    int editPos = 0;
};

#endif // PATTERNEDITOR_H
