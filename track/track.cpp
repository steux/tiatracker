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

void Track::newTrack() {
    name = "new track.ttt";
    instruments = QList<Instrument>{
        {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"}
    };
    percussion = QList<Percussion>{
        {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"},
        {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"}, {"---"}
    };

    patterns.clear();
    Pattern newPattern("New pattern");
    Note newNote(Note::instrumentType::Hold, 0, 0);
    for (int i = 0; i < 16; ++i) {
        newPattern.notes.append(newNote);
    }
    patterns.append(newPattern);
    channelSequences = QList<Sequence>{{}, {}};
    SequenceEntry newEntry(0, -1);
    channelSequences[0].sequence.append(newEntry);
    channelSequences[1].sequence.append(newEntry);
    evenSpeed = 5;
    oddSpeed = 5;
    rowsPerBeat = 4;
    startPatterns[0] = 0;
    startPatterns[1] = 0;
    playPos[0] = 0;
    playPos[1] = 0;
    tvMode = TiaSound::TvStandard::PAL;
    updateFirstNoteNumbers();
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

int Track::getChannelNumRows(int channel) {
    int lastPattern = channelSequences[channel].sequence.last().patternIndex;
    return channelSequences[channel].sequence.last().firstNoteNumber
            + patterns[lastPattern].notes.size();
}

/*************************************************************************/

int Track::getTrackNumRows() {
    return std::max(getChannelNumRows(0), getChannelNumRows(1));
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

int Track::getSequenceEntryIndex(int channel, int row) {
    int entryIndex = 0;
    SequenceEntry *curEntry = &(channelSequences[channel].sequence[0]);
    Pattern *curPattern = &(patterns[curEntry->patternIndex]);
    while (row >= curEntry->firstNoteNumber + curPattern->notes.size()) {
        entryIndex++;
        curEntry = &(channelSequences[channel].sequence[entryIndex]);
        curPattern = &(patterns[curEntry->patternIndex]);
    }
    return entryIndex;
}

/*************************************************************************/

int Track::getPatternIndex(int channel, int row) {
    int entryIndex = getSequenceEntryIndex(channel, row);
    return channelSequences[channel].sequence[entryIndex].patternIndex;
}

/*************************************************************************/

int Track::getNoteIndexInPattern(int channel, int row) {
    int entryIndex = getSequenceEntryIndex(channel, row);
    return row - channelSequences[channel].sequence[entryIndex].firstNoteNumber;
}

/*************************************************************************/

bool Track::getNextNote(int channel, int *pEntryIndex, int *pPatternNoteIndex) {
    SequenceEntry *curEntry = &(channelSequences[channel].sequence[*pEntryIndex]);
    Pattern *curPattern = &(patterns[curEntry->patternIndex]);
    (*pPatternNoteIndex)++;
    if (*pPatternNoteIndex == curPattern->notes.size()) {
        *(pEntryIndex) = *(pEntryIndex) + 1;
        if (*pEntryIndex == channelSequences[channel].sequence.size()) {
            // End of track reached: There is no next note
            return false;
        }
        *(pPatternNoteIndex) = 0;
    }
    return true;
}

/*************************************************************************/

bool Track::getNextNoteWithGoto(int channel, int *pEntryIndex, int *pPatternNoteIndex) {
    SequenceEntry *curEntry = &(channelSequences[channel].sequence[*pEntryIndex]);
    Pattern *curPattern = &(patterns[curEntry->patternIndex]);
    (*pPatternNoteIndex)++;
    if (*pPatternNoteIndex == curPattern->notes.size()) {
        if (curEntry->gotoTarget == -1) {
            *(pEntryIndex) = *(pEntryIndex) + 1;
        } else {
            *(pEntryIndex) = curEntry->gotoTarget;
        }
        if (*pEntryIndex >= channelSequences[channel].sequence.size()) {
            // End of track or invalid goto: There is no next note
            return false;
        }
        *(pPatternNoteIndex) = 0;
    }
    return true;
}

/*************************************************************************/

bool Track::checkSlideValidity(int channel, int row) {
    // Skip slides and holds immediately before
    int prevRow = row;
    Note *n;
    do {
        prevRow = skipInstrumentType(channel, prevRow, Note::instrumentType::Slide, -1);
        if (prevRow == -1) {
            return false;
        }
        n = getNote(channel, prevRow);
    } while(n->type == Note::instrumentType::Hold);

    if (n->type != Note::instrumentType::Instrument) {
        return false;
    }
    return true;
}

/*************************************************************************/

int Track::skipInstrumentType(int channel, int row, Note::instrumentType type, int direction) {
    int channelRows = getChannelNumRows(channel);
    do {
        row += direction;
        if (row == -1 || row == channelRows) {
            return -1;
        }
    } while (getNote(channel, row)->type == type);
    return row;
}

/*************************************************************************/

Note *Track::getNote(int channel, int row) {
    int entryIndex = getSequenceEntryIndex(channel, row);
    int patternIndex = channelSequences[channel].sequence[entryIndex].patternIndex;
    int noteIndex = row - channelSequences[channel].sequence[entryIndex].firstNoteNumber;
    return &(patterns[patternIndex].notes[noteIndex]);
}

/*************************************************************************/

void Track::validatePattern(int patternIndex) {
    Note::instrumentType mode = Note::instrumentType::Hold;
    Pattern *pattern = &(patterns[patternIndex]);
    for (int i = 0; i < pattern->notes.size(); ++i) {
        Note::instrumentType curType = pattern->notes[i].type;
        switch (curType) {
        case Note::instrumentType::Hold:
            break;
        case Note::instrumentType::Instrument:
            mode = Note::instrumentType::Instrument;
            break;
        case Note::instrumentType::Pause:
            if (mode != Note::instrumentType::Instrument) {
                pattern->notes[i].type = Note::instrumentType::Hold;
            }
            mode = Note::instrumentType::Hold;
            break;
        case Note::instrumentType::Percussion:
            mode = Note::instrumentType::Percussion;
            break;
        case Note::instrumentType::Slide:
            if (mode != Note::instrumentType::Instrument) {
                mode = Note::instrumentType::Hold;
                pattern->notes[i].type = Note::instrumentType::Hold;
            }
            break;
        }
    }
}

/*************************************************************************/

bool Track::usesGoto() {
    for (int channel = 0; channel < 2; ++channel) {
        for (int i = 0; i < channelSequences[channel].sequence.size(); ++i) {
            if (channelSequences[channel].sequence[i].gotoTarget != -1) {
                return true;
            }
        }
    }
    return false;
}

/*************************************************************************/

bool Track::startsWithHold() {
    return (getNote(0, 0)->type == Note::instrumentType::Hold && getNote(1, 0)->type == Note::instrumentType::Hold);
}

/*************************************************************************/

bool Track::usesSlide() {
    for (int channel = 0; channel < 2; ++channel) {
        for (int i = 0; i < getChannelNumRows(channel); ++i) {
            if (getNote(channel, i)->type == Note::instrumentType::Slide) {
                return true;
            }
        }
    }
    return false;
}

/*************************************************************************/

bool Track::usesOverlay() {
    for (int channel = 0; channel < 2; ++channel) {
        for (int i = 0; i < getChannelNumRows(channel); ++i) {
            Note *n = getNote(channel, i);
            if (n->type == Note::instrumentType::Percussion && percussion[n->instrumentNumber].overlay) {
                return true;
            }
        }
    }
    return false;
}

/*************************************************************************/

bool Track::usesFunktempo() {
    return (evenSpeed != oddSpeed);
}

/*************************************************************************/

int Track::getNumInstrumentsFromTrack() {
    QList<int> found{0, 0, 0, 0, 0, 0, 0};
    for (int channel = 0; channel < 2; ++channel) {
        for (int i = 0; i < getChannelNumRows(channel); ++i) {
            Note *n = getNote(channel, i);
            if (n->type == Note::instrumentType::Instrument) {
                if (instruments[n->instrumentNumber].baseDistortion == TiaSound::Distortion::PURE_COMBINED) {
                    found[n->instrumentNumber] = 2;
                } else {
                    found[n->instrumentNumber] = 1;
                }
            }
        }
    }
    int result = 0;
    for (int i = 0; i < numInstruments; ++i) {
        result += found[i];
    }
    return result;
}

/*************************************************************************/

int Track::getNumPercussionFromTrack() {
    QList<int> found{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int channel = 0; channel < 2; ++channel) {
        for (int i = 0; i < getChannelNumRows(channel); ++i) {
            Note *n = getNote(channel, i);
            if (n->type == Note::instrumentType::Percussion) {
                found[n->instrumentNumber] = 1;
            }
        }
    }
    int result = 0;
    for (int i = 0; i < numPercussion; ++i) {
        result += found[i];
    }
    return result;
}

/*************************************************************************/

int Track::calcInstrumentsSize() {
    QList<int> found{0, 0, 0, 0, 0, 0, 0};
    for (int channel = 0; channel < 2; ++channel) {
        for (int i = 0; i < getChannelNumRows(channel); ++i) {
            Note *n = getNote(channel, i);
            if (n->type == Note::instrumentType::Instrument) {
                if (instruments[n->instrumentNumber].baseDistortion == TiaSound::Distortion::PURE_COMBINED) {
                    found[n->instrumentNumber] = 2;
                } else {
                    found[n->instrumentNumber] = 1;
                }
            }
        }
    }
    int result = 0;
    for (int i = 0; i < numInstruments; ++i) {
        if (found[i] != 0) {
            result += found[i]*Emulation::Player::RomPerInstrument;
            result += instruments[i].calcEffectiveSize();
        }
    }
    return result;
}

/*************************************************************************/

int Track::calcPercussionSize() {
    QList<int> found{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int channel = 0; channel < 2; ++channel) {
        for (int i = 0; i < getChannelNumRows(channel); ++i) {
            Note *n = getNote(channel, i);
            if (n->type == Note::instrumentType::Percussion) {
                found[n->instrumentNumber] = 1;
            }
        }
    }
    int result = 0;
    for (int i = 0; i < numPercussion; ++i) {
        if (found[i] != 0) {
            result += Emulation::Player::RomPerPercussion;
            result += 2*percussion[i].calcEffectiveSize();
        }
    }
    return result;
}

/*************************************************************************/

int Track::numPatternsUsed() {
    QMap<int, bool> found{};
    for (int channel = 0; channel < 2; ++channel) {
        for (int i = 0; i < channelSequences[channel].sequence.size(); ++i) {
            found[channelSequences[channel].sequence[i].patternIndex] = true;
        }
    }
    QMapIterator<int, bool> i(found);
    while (i.hasNext()) {
        i.next();
    }
    return found.size();
}

/*************************************************************************/

int Track::calcPatternSize() {
    QMap<int, bool> found;
    for (int channel = 0; channel < 2; ++channel) {
        for (int i = 0; i < channelSequences[channel].sequence.size(); ++i) {
            found[channelSequences[channel].sequence[i].patternIndex] = true;
        }
    }
    int result = 0;
    QMapIterator<int, bool> i(found);
    while (i.hasNext()) {
        i.next();
        result += Emulation::Player::RomPerPattern + patterns[i.key()].notes.size();
    }
    return result;
}

/*************************************************************************/

int Track::sequencesSize() {
    return channelSequences[0].sequence.size() + channelSequences[1].sequence.size()
            + 2*Emulation::Player::RomPerSequence;
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

    // Sequences
    QJsonArray chanArray;
    for (int i = 0; i < 2; ++i) {
        QJsonObject chanJson;
        channelSequences[i].toJson(chanJson);
        chanArray.append(chanJson);
    }
    json["channels"] = chanArray;
}

/*************************************************************************/

bool Track::fromJson(const QJsonObject &json) {
    int version = json["version"].toInt();
    if (version > MainWindow::version) {
        return false;
    }
    if (json["tvmode"] == "pal") {
        tvMode = TiaSound::TvStandard::PAL;
    } else if (json["tvmode"] == "ntsc") {
        tvMode = TiaSound::TvStandard::NTSC;
    } else {
        return false;
    }
    evenSpeed = json["evenspeed"].toInt();
    oddSpeed = json["oddspeed"].toInt();
    rowsPerBeat = json["rowsperbeat"].toInt();

    // Instruments
    QJsonArray insArray = json["instruments"].toArray();
    if (insArray.size() != numInstruments) {
        return false;
    }
    instruments.clear();
    for (int i = 0; i < numInstruments; ++i) {
        Instrument newIns{""};
        if (!newIns.import(insArray[i].toObject())) {
            return false;
        }
        instruments.append(newIns);
    }

    // Percussion
    QJsonArray percArray = json["percussion"].toArray();
    if (percArray.size() != numPercussion) {
        return false;
    }
    percussion.clear();
    for (int i = 0; i < numPercussion; ++i) {
        Percussion newPerc{""};
        if (!newPerc.import(percArray[i].toObject())) {
            return false;
        }
        percussion.append(newPerc);
    }

    // Patterns
    QJsonArray pattArray = json["patterns"].toArray();
    patterns.clear();
    for (int i = 0; i < pattArray.size(); ++i) {
        Pattern newPatt;
        if (!newPatt.fromJson(pattArray[i].toObject())) {
            return false;
        }
        patterns.append(newPatt);
    }

    // Sequences
    QJsonArray chanArray = json["channels"].toArray();
    if (chanArray.size() != 2) {
        return false;
    }
    channelSequences.clear();
    for (int i = 0; i < 2; ++i) {
        Sequence newSeq;
        if (!newSeq.fromJson(chanArray[i].toObject())) {
            return false;
        }
        channelSequences.append(newSeq);
    }

    updateFirstNoteNumbers();
    return true;
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
