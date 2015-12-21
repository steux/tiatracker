#include <QThread>
#include <QTimer>
#include <iostream>
#include "track/track.h"
#include "track/instrument.h"
#include "player.h"
#include <SDL.h>
#include <QElapsedTimer>

namespace Emulation {

Player::Player(Track::Track *parentTrack, QObject *parent) : QThread(parent)
{
    pTrack = parentTrack;

    tiaSound.channels(2, false);
    sdlSound.setFrameRate(50.0);
    sdlSound.open();
    sdlSound.mute(false);
    sdlSound.setEnabled(true);
    sdlSound.setVolume(100);

    sdlSound.set(AUDC0, 0, 10);
    sdlSound.set(AUDV0, 0, 15);
    sdlSound.set(AUDF0, 0, 18);
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
    timer->start(1000/50);
}

/*************************************************************************/

void Player::setFrameRate(float rate) {
    sdlSound.close();
    sdlSound.setFrameRate(rate);
    sdlSound.open();
    timer->setInterval(int(1000/rate));
}

/*************************************************************************/

void Player::playInstrument(Track::Instrument *instrument, int frequency) {
    currentInstrument = instrument;
    currentInstrumentFrequency = frequency;
    currentInstrumentFrame = 0;
    mode = PlayMode::INSTRUMENT;
}

/*************************************************************************/

void Player::stopInstrument() {
    currentInstrumentFrame = currentInstrument->getReleaseStart();
}

/*************************************************************************/

void Player::updateSilence() {
    sdlSound.set(AUDC0, 0, 10);
    sdlSound.set(AUDV0, 0, 15);
    sdlSound.set(AUDF0, 0, 18);
}

void Player::updateInstrument() {
    /* Play current frame */
    // Check if instrument has changed and made currentFrame illegal
    pTrack->lock();
    if (currentInstrumentFrame >= currentInstrument->getEnvelopeLength()) {
        // Go into silence if currentFrame is illegal
        mode = PlayMode::NONE;
    } else {
        int CValue = (static_cast<int>(currentInstrument->baseDistortion));
        int FValue = currentInstrumentFrequency + currentInstrument->frequencies[currentInstrumentFrame];
        // Check if envelope has caused an underrun
        if (FValue < 0) {
            FValue = 256 - FValue;
        }
        int VValue = currentInstrument->volumes[currentInstrumentFrame];
        sdlSound.set(AUDC0, CValue, 10);
        sdlSound.set(AUDV0, VValue, 15);
        sdlSound.set(AUDF0, FValue, 18);

        /* Advance frame */
        currentInstrumentFrame++;
        // Check for end of sustain or release
        if (currentInstrumentFrame == currentInstrument->getReleaseStart()) {
            // Remain in sustain
            currentInstrumentFrame = currentInstrument->getSustainStart();
        } else if (currentInstrumentFrame == currentInstrument->getEnvelopeLength()) {
            // End of release: Go into silence
            mode = PlayMode::NONE;
        }
    }
    pTrack->unlock();
}

void Player::timerFired() {
    switch (mode) {
    case PlayMode::INSTRUMENT:
        updateInstrument();
        break;
    default:
        updateSilence();
    }
}

}
