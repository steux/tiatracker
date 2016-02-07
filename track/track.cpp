#include "track.h"
#include <QString>
#include "sequenceentry.h"
#include <iostream>
#include "mainwindow.h"
#include <QJsonArray>


namespace Track {

Track::Track() {

}

/*************************************************************************/

void Track::lock() {
    mutex.lock();
}

/*************************************************************************/

void Track::unlock() {
    mutex.unlock();
}

/*************************************************************************/

int Track::getNumUsedEnvelopeFrames() {
    int numFrames = 0;
    for (int i = 0; i < numInstruments; ++i) {
        if (!instruments[i].isEmpty()) {
            int envelopeLength = instruments[i].getEnvelopeLength();
            numFrames += envelopeLength;
            // If last volume/frequency frames are not 0, add one (0 is mandatory)
            int iLast = envelopeLength - 1;
            if (instruments[i].volumes[iLast] != 0 || instruments[i].frequencies[iLast] != 0) {
                numFrames++;
            }
        }
    }
    return numFrames;
}

/*************************************************************************/

int Track::getNumUsedPercussionFrames() {
    int numFrames = 0;
    for (int i = 0; i < numPercussion; ++i) {
        if (!percussion[i].isEmpty()) {
            int envelopeLength = percussion[i].getEnvelopeLength();
            numFrames += envelopeLength;
            // If last ctrl/volume frames are not 0, add one (0 is mandatory)
            int iLast = envelopeLength - 1;
            if (percussion[i].volumes[iLast] != 0
                    || percussion[i].waveforms[iLast] != TiaSound::Distortion::SILENT) {
                numFrames++;
            }
        }
    }
    return numFrames;
}

/*************************************************************************/

int Track::getNumUsedInstruments() {
    int usedInstruments = 0;
    for (int i = 0; i < numInstruments; ++i) {
        if (!instruments[i].isEmpty()) {
            usedInstruments++;
            if (instruments[i].baseDistortion == TiaSound::Distortion::PURE_COMBINED) {
                usedInstruments++;
            }
        }
    }
    return usedInstruments;
}

/*************************************************************************/

int Track::getNumUsedPercussion()
{
    int usedPercussion = 0;
    for (int i = 0; i < numPercussion; ++i) {
        if (!percussion[i].isEmpty()) {
            usedPercussion++;
        }
    }
    return usedPercussion;
}

/*************************************************************************/

int Track::getNumRows() {
    int lastPattern0 = channelSequences[0].sequence.last().patternIndex;
    int size0 = channelSequences[0].sequence.last().firstNoteNumber +
            patterns[lastPattern0].notes.size();
    int lastPattern1 = channelSequences[1].sequence.last().patternIndex;
    int size1 = channelSequences[1].sequence.last().firstNoteNumber +
            patterns[lastPattern1].notes.size();
    return std::max(size0, size1);
}

/*************************************************************************/

void Track::updateFirstNoteNumbers() {
    for (int channel = 0; channel < 2; ++channel) {
        int noteNumber = 0;
        for (int entry = 0; entry < channelSequences[channel].sequence.size(); ++entry) {
            channelSequences[channel].sequence[entry].firstNoteNumber = noteNumber;
            int iPattern = channelSequences[channel].sequence[entry].patternIndex;
            noteNumber += patterns[iPattern].notes.size();
        }
    }
}

/*************************************************************************/

void Track::toJson(QJsonObject &json) {
    // General data
    json["version"] = MainWindow::version;
    if (tvMode == TiaSound::TvStandard::PAL) {
        json["tvmode"] = "pal";
    } else {
        json["tvmode"] = "ntsc";
    }
    json["evenspeed"] = evenSpeed;
    json["oddspeed"] = oddSpeed;
    json["rowsperbeat"] = rowsPerBeat;

    // Instruments
    QJsonArray insArray;
    for (int i = 0; i < numInstruments; ++i) {
        QJsonObject insJson;
        instruments[i].toJson(insJson);
        insArray.append(insJson);
    }
    json["instruments"] = insArray;

    // Percussion
    QJsonArray percArray;
    for (int i = 0; i < numPercussion; ++i) {
        QJsonObject percJson;
        percussion[i].toJson(percJson);
        percArray.append(percJson);
    }
    json["percussion"] = percArray;

    // Patterns
    QJsonArray pattArray;
    for (int i = 0; i < patterns.size(); ++i) {
        QJsonObject pattJson;
        patterns[i].toJson(pattJson);
        pattArray.append(pattJson);
    }
    json["patterns"] = pattArray;

    // Sequence
}

/*************************************************************************/

bool Track::fromJson(const QJsonObject &json) {

}

/*************************************************************************/

TiaSound::TvStandard Track::getTvMode() const {
    return tvMode;
}

/*************************************************************************/

void Track::setTvMode(const TiaSound::TvStandard &value) {
    tvMode = value;
}

}
