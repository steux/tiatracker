#include <QThread>
#include <QTimer>
#include <iostream>

#include "player.h"
#include <SDL.h>

namespace Emulation {

Player::Player(QObject *parent) : QThread(parent)
{
    //moveToThread(this);
}

Player::~Player()
{
    delete timer;
}

/*************************************************************************/

void Player::run() {

    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timerFired()));
    timer->start(1000);
}

/*************************************************************************/

void Player::playInstrument(Track::Instrument *instrument, int frequency) {
    std::cout << "playInstrument: " << instrument->name.toStdString() << ", " << frequency << "\n"; std::cout.flush();
}

/*************************************************************************/

void Player::stopInstrument() {
    std::cout << "stopInstrument\n"; std::cout.flush();
}

/*************************************************************************/

void Player::timerFired() {
    std::cout << "TIMER\n"; std::cout.flush();
}

}