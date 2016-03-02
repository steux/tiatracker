#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QThread>
#include <QTimer>

#include "track/track.h"
#include "track/instrument.h"
#include "tiasound/tiasound.h"
#include "emulation/TIASnd.h"
#include "emulation/SoundSDL2.h"
#include <QElapsedTimer>


namespace Emulation {

class Player : public QThread {
    Q_OBJECT

public:
    explicit Player(Track::Track *parentTrack, QObject *parent = 0);
    ~Player();

    /* Start the thread, i.e. initialize */
    void run();

    /* Set framerate to play at */
    void setFrameRate(float rate);

public slots:
    // Stop everything next frame
    void silence();

    /* Start to play a note with a given instrument */
    void playInstrument(Track::Instrument *instrument, int frequency);
    void playInstrumentOnce(Track::Instrument *instrument, int frequency);
    /* Stop playing a note, i.e. send instrument into release */
    void stopInstrument();

    /* Start and stop playing a percussion */
    void playPercussion(Track::Percussion *percussion);
    void stopPercussion();

    void stopTrack();

    /* Start waveform */
    void playWaveform(TiaSound::Distortion waveform, int frequency, int volume);

    /* Play song from given channel note indexes */
    void playTrack(int start1, int start2);

signals:
    void newPlayerPos(int pos1, int pos2);
    void invalidNoteFound(int channel, int entryIndex, int noteIndex);

private:
    Track::Track *pTrack = nullptr;
    Emulation::TIASound tiaSound;
    Emulation::SoundSDL2 sdlSound{&tiaSound};

    QTimer *timer = nullptr;

    // Play mode we are in
    enum class PlayMode {
        None, Instrument, InstrumentOnce, Percussion, Waveform, Track
    };
    PlayMode mode = PlayMode::None;

    /* Current values for instrument play */
    Track::Instrument *currentInstrument;
    int currentInstrumentFrequency;
    int currentInstrumentFrame;

    /* Current values for percussion play */
    Track::Percussion *currentPercussion;
    int currentPercussionFrame;

    /* Helper methods for timerFired() */
    void updateSilence();
    void updateInstrument();
    void updatePercussion();
    // Get and process next note in channel
    void sequenceChannel(int channel);
    // Play current note in channel
    void updateChannel(int channel);
    // Do next tick for track
    void updateTrack();

    /* Set values for channel 0 */
    void setChannel0(int distortion, int frequency, int volume);

    /* Set values for a channel */
    void setChannel(int channel, int distortion, int frequency, int volume);

    /* Play track vars */
    // current note index inside pattern
    int trackCurNoteIndex[2];
    // Current entry in sequence
    int trackCurEntryIndex[2];
    int trackCurTick;
    Track::Note trackCurNote[2];
    int trackCurEnvelopeIndex[2];
    // Current mode, to validate track
    Track::Note::instrumentType trackMode[2];

private slots:
    void timerFired();
};

}

#endif // PLAYER_H
