#ifndef WAVEFORMSHAPER_H
#define WAVEFORMSHAPER_H

#include <QWidget>
#include <QString>
#include <QList>
#include "track/track.h"

/* GUI element for displaying and manipulating envelope data.
 */
class WaveformShaper : public QWidget
{
    Q_OBJECT
public:
    static const int widgetHeight = 196;

    explicit WaveformShaper(QWidget *parent);

    /* Register the track with the instruments to modify */
    void registerInstrument(Track::Instrument *newInstrument);

    /* Set fixed size for layout according to envelope length */
    void updateSize();

    /* Getters/Setters */
    void setScale(int min, int max);
    QList<int> *getValues();
    void setValues(QList<int> *newValues);

    QString name{};

signals:

    /* Gets emitted when the mouse is released */
    void newMaxValue(int newMax);

public slots:

protected:
    static const int legendCellSize = 20;
    static const int cellWidth = 16;

    void drawLegend(QPainter &painter, const int valuesXPos, const int valuesHeight);
    void drawAttackDecay(QPainter &painter, const int valuesXPos, const int valuesHeight);
    void drawWaveform(QPainter &painter, const int valuesXPos);
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

    void processMouseEvent(int x, int y);
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    int calcWidth();

    // The instrument to edit
    Track::Instrument *pInstrument = nullptr;

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

#endif // WAVEFORMSHAPER_H
