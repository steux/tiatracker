#ifndef INSTRUMENTSELECTOR_H
#define INSTRUMENTSELECTOR_H

#include <QObject>
#include <QWidget>
#include <QFont>
#include "track/track.h"


class InstrumentSelector : public QWidget
{
    Q_OBJECT
public:
    explicit InstrumentSelector(QWidget *parent = 0);

    void registerTrack(Track::Track *newTrack);

    QSize sizeHint() const;

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    static const int horizontalMargin = 10;
    static const int verticalMargin = 10;
    static const int buttonHorizontalMargin = 6;
    static const int buttonVerticalMargin = 1;
    static const int insPercMargin = 8;
    static const int fontSize = 12;
    static const int minWidth = 120;


    Track::Track *pTrack = nullptr;

    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    QFont font{"Helvetica"};
    int widgetHeight;
    int fontHeight;
    int buttonWidth;
    int buttonHeight;

    int selected = 0;
};

#endif // INSTRUMENTSELECTOR_H
