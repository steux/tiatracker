#ifndef PERCUSSIONSHAPER_H
#define PERCUSSIONSHAPER_H

#include <QWidget>
#include <QString>
#include <QList>
#include "track/track.h"

/* GUI element for displaying and manipulating percussion frames.
 */
class PercussionShaper : public QWidget
{
    Q_OBJECT
public:
    // Frequency has 32 cells, +20 for legendCellSize
    static const int widgetHeight = 180;

    explicit PercussionShaper(QWidget *parent);

    /* Register the instrument to modify */
    void registerPercussion(Track::Percussion *newPercussion);

    /* Set fixed size for layout according to envelope length */
    void updateSize();

    /* Getters/Setters */
    void setScale(int min, int max);
    QList<int> *getValues();
    void setValues(QList<int> *newValues);

    QString name{};
    // Invert y axis
    bool isInverted = false;

signals:
    /* Gets emitted when a new value is set while dragging the mouse */
    void newPercussionValue(int iFrame);
    /* Gets emitted when the mouse is released, to stop waveform play */
    void silence();
    /* Gets emitted when the mouse is released */
    void newMaxValue(int newMax);

public slots:

protected:
    static const int legendCellSize = 20;
    static const int cellWidth = 16;

    void drawLegend(QPainter &painter, const int valuesXPos, const int valuesHeight);
    void drawWaveform(QPainter &painter, const int valuesXPos);
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

    void processMouseEvent(int x, int y);
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    int calcWidth();

    // The percussion to edit
    Track::Percussion *pPercussion = nullptr;

    int scaleMin = 0;
    int scaleMax = 15;
    QList<int> *values = nullptr;
    int cellHeight;
    bool isMouseDragging = false;
    int draggingIndex = -1;

    static const int legendScaleSize = 11;
    static const int legendNameSize = 17;
    QFont legendFont{"Helvetica"};
    int nameFontHeight;
    static const int valueCircleRadius = 4;

};

#endif // PERCUSSION_H
