#ifndef WAVEFORMSHAPER_H
#define WAVEFORMSHAPER_H

#include <QWidget>
#include <QString>
#include <QList>
#include "track/track.h"

class WaveformShaper : public QWidget
{
    Q_OBJECT
public:
    static const int widgetHeight = 196;

    QString name{};

    explicit WaveformShaper(QWidget *parent);

    // Register the track with the instruments to modify
    void registerTrack(Track::Track *newPTrack) {pTrack = newPTrack;}

    void setScale(int min, int max);
    int getSustainStart() const;
    void setSustainStart(int value);
    int getReleaseStart() const;
    void setReleaseStart(int value);

    QList<int> getValues() const;
    void setValues(const QList<int> &value);

signals:

public slots:

protected:
    static const int legendCellSize = 20;
    static const int cellWidth = 16;

    void drawLegend(QPainter &painter, const int valuesXPos, const int valuesHeight);
    void drawAttackDecay(QPainter &painter, const int valuesXPos, const int valuesHeight);
    void drawWaveform(QPainter &painter, const int valuesXPos);
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    // The Track containing the instruments to edit
    Track::Track *pTrack = nullptr;

    int scaleMin = 0;
    int scaleMax = 15;
    QList<int> values{};
    int cellHeight;
    int sustainStart;
    int releaseStart;

    static const int legendScaleSize = 11;
    static const int legendNameSize = 17;
    QFont legendFont{"Helvetica"};
    int nameFontHeight;
    static const int valueCircleRadius = 4;

    int calcWidth() { return legendCellSize + values.size()*cellWidth; }
};

#endif // WAVEFORMSHAPER_H
