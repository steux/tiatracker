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

void Player::silence() {
    mode = PlayMode::None;
}

/*************************************************************************/

void Player::playInstrument(Track::Instrument *instrument, int frequency) {
    currentInstrument = instrument;
    currentInstrumentFrequency = frequency;
    currentInstrumentFrame = 0;
    mode = PlayMode::Instrument;
}

/*************************************************************************/

void Player::playInstrumentOnce(Track::Instrument *instrument, int frequency) {
    currentInstrument = instrument;
    currentInstrumentFrequency = frequency;
    currentInstrumentFrame = 0;
    mode = PlayMode::InstrumentOnce;
}

/*************************************************************************/

void Player::stopInstrument() {
    currentInstrumentFrame = currentInstrument->getReleaseStart();
}

/*************************************************************************/

void Player::playPercussion(Track::Percussion *percussion) {
    currentPercussion = percussion;
    currentPercussionFrame = 0;
    mode = PlayMode::Percussion;
}

/*************************************************************************/

void Player::stopPercussion() {
    mode = PlayMode::None;
}

/*************************************************************************/

void Player::setChannel0(int distortion, int frequency, int volume) {
    sdlSound.set(AUDC0, distortion, 10);
    sdlSound.set(AUDV0, volume, 15);
    sdlSound.set(AUDF0, frequency, 18);
}

/*************************************************************************/

void Player::playWaveform(TiaSound::Distortion waveform, int frequency, int volume) {
    int distortion = TiaSound::getDistortionInt(waveform);
    setChannel0(distortion, frequency, volume);
    mode = PlayMode::Waveform;
}

/*************************************************************************/

void Player::playTrack(int start1, int start2) {
    trackCurNoteIndex[0] = pTrack->getNoteIndexInPattern(0, start1);
    trackCurNoteIndex[1] = pTrack->getNoteIndexInPattern(1, start2);
    trackCurEntryIndex[0] = pTrack->getSequenceEntryIndex(0, start1);
    trackCurEntryIndex[1] = pTrack->getSequenceEntryIndex(0, start2);
    trackCurTick[0] = 0;
    trackCurTick[1] = 0;
    mode = PlayMode::Track;
}

/*************************************************************************/

void Player::updateSilence() {
    setChannel0(0, 0, 0);
}

/*************************************************************************/

void Player::updateInstrument() {
    /* Play current frame */
    // Check if instrument has changed and made currentFrame illegal
    pTrack->lock();
    if (currentInstrumentFrame >= currentInstrument->getEnvelopeLength()) {
        // Go into silence if currentFrame is illegal
        mode = PlayMode::None;
    } else {
        int CValue = currentInstrument->getAudCValue(currentInstrumentFrequency);
        int realFrequency = currentInstrumentFrequency <= 31 ? currentInstrumentFrequency : currentInstrumentFrequency - 32;
        int FValue = realFrequency + currentInstrument->frequencies[currentInstrumentFrame];
        // Check if envelope has caused an underrun
        if (FValue < 0) {
            FValue = 256 + FValue;
        }
        int VValue = currentInstrument->volumes[currentInstrumentFrame];
        setChannel0(CValue, FValue, VValue);

        /* Advance frame */
        currentInstrumentFrame++;
        // Check for end of sustain or release
        if (mode != PlayMode::InstrumentOnce
                && currentInstrumentFrame == currentInstrument->getReleaseStart()) {
            currentInstrumentFrame = currentInstrument->getSustainStart();
        } else if (currentInstrumentFrame == currentInstrument->getEnvelopeLength()) {
            // End of release: Go into silence
            mode = PlayMode::None;
        }
    }
    pTrack->unlock();
}

/*************************************************************************/

void Player::updatePercussion() {
    /* Play current frame */
    pTrack->lock();
    if (currentPercussionFrame >= currentPercussion->getEnvelopeLength()) {
        // Go into silence if currentFrame is illegal
        mode = PlayMode::None;
    } else {
        TiaSound::Distortion waveform = currentPercussion->waveforms[currentPercussionFrame];
        int CValue = TiaSound::getDistortionInt(waveform);
        int FValue = currentPercussion->frequencies[currentPercussionFrame];
        int VValue = currentPercussion->volumes[currentPercussionFrame];
        setChannel0(CValue, FValue, VValue);

        /* Advance frame. End of waveform is handled next frame */
        currentPercussionFrame++;
    }
    pTrack->unlock();
}

/*************************************************************************/

void Player::updateTrack() {
    // TODO
}

/*************************************************************************/

void Player::timerFired() {
    switch (mode) {
    case PlayMode::Instrument:
    case PlayMode::InstrumentOnce:
        updateInstrument();
        break;
    case PlayMode::Waveform:
        break;
    case PlayMode::Percussion:
        updatePercussion();
        break;
    case PlayMode::Track:
        updateTrack();
        break;
    default:
        updateSilence();
    }
}

}
