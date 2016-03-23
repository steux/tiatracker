#ifndef OPTIONSTAB_H
#define OPTIONSTAB_H

#include <QObject>
#include <QWidget>
#include "track/track.h"
#include "emulation/player.h"


class OptionsTab : public QWidget
{
    Q_OBJECT
public:
    explicit OptionsTab(QWidget *parent = 0);

    void registerTrack(Track::Track *newTrack);

    /* Initializes the GUI components. Must be called once during init. */
    void initOptionsTab();

    /* Fills GUI elements with data from the track. Called upon changes. */
    void updateOptionsTab();


signals:

public slots:

private:

    Track::Track *pTrack = nullptr;

};

#endif // OPTIONSTAB_H
