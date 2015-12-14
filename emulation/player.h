#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QThread>
#include <QTimer>

#include "track/instrument.h"
#include "tiasound/tiasound.h"
#include "emulation/TIASnd.h"
#include "emulation/SoundSDL2.h"

namespace Emulation {

class Player : public QThread {
    Q_OBJECT

public:
    explicit Player(QObject *parent = 0);
    ~Player();

    /* Start the thread, i.e. initialize */
    void run();

public slots:
    /* Start to play a note with a given instrument */
    void playInstrument(Track::Instrument *instrument, int frequency);
    /* Stop playing a note */
    void stopInstrument();

signals:

private:
    Emulation::TIASound tiaSound{31113};    // FIXME
    Emulation::SoundSDL2 sdlSound{&tiaSound};

    QTimer *timer = nullptr;

private slots:
    void timerFired();
};

}

#endif // PLAYER_H
