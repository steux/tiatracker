#ifndef WAVEFORMSHAPER_H
#define WAVEFORMSHAPER_H

#include <QObject>
#include <QWidget>
#include <QList>
#include "track/track.h"
#include "tiasound/tiasound.h"

class WaveformShaper : public QWidget
{
    Q_OBJECT
public:
    explicit WaveformShaper(QWidget *parent = 0);

    /* Register the instrument to modify */
    void registerPercussion(Track::Percussion *newPercussion);

    /* Set fixed size for layout according to envelope length */
    void updateSize();

    QList<int> *getValues();
    void setValues(QList<int> *newValues);

signals:

public slots:

protected:
    static const int legendCellSize = 20;
    static const int cellWidth = 16;

    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    int calcWidth();

    // The percussion to edit
    Track::Percussion *pPercussion = nullptr;

    QList<int> *values = nullptr;
    int cellHeight;

    static const int valueFontSize = 11;
    QFont valueFont{"Helvetica"};
    int valueFontHeight;
};

#endif // WAVEFORMSHAPER_H
