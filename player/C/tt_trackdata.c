// TIATracker music player
// Copyright 2016 Andre "Kylearan" Wichmann
// Website: https://bitbucket.org/kylearan/tiatracker
// Email: andre.wichmann@gmx.de
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Song author: %%AUTHOR%%
// Song name: %%NAME%%

// @com.wudsn.ide.asm.hardware=ATARI2600

// =====================================================================
// TIATracker melodic and percussion instruments, patterns and sequencer
// data.
// =====================================================================

// =====================================================================
// Melodic instrument definitions (up to 7). tt_envelope_index_c0/1 hold
// the index values into these tables for the current instruments played
// in channel 0 and 1.
// 
// Each instrument is defined by:
// - tt_InsCtrlTable: the AUDC value
// - tt_InsADIndexes: the index of the start of the ADSR envelope as
//       defined in tt_InsFreqVolTable
// - tt_InsSustainIndexes: the index of the start of the Sustain phase
//       of the envelope
// - tt_InsReleaseIndexes: the index of the start of the Release phase
// - tt_InsFreqVolTable: The AUDF frequency and AUDV volume values of
//       the envelope
// =====================================================================

// Instrument master CTRL values
const char tt_InsCtrlTable[] = {
%%INSCTRLTABLE%%
};

// Instrument Attack/Decay start indexes into ADSR tables.
const char tt_InsADIndexes[] = {
%%INSADINDEXES%%
};

// Instrument Sustain start indexes into ADSR tables
const char tt_InsSustainIndexes[] = {
%%INSSUSTAININDEXES%%
};

// Instrument Release start indexes into ADSR tables
// Caution: Values are stored with an implicit -1 modifier! To get the
// real index, add 1.
const char tt_InsReleaseIndexes[] = {
%%INSRELEASEINDEXES%%
};

// AUDVx and AUDFx ADSR envelope values.
// Each byte encodes the frequency and volume:
// - Bits 7..4: Freqency modifier for the current note ([-8..7]),
//       8 means no change. Bit 7 is the sign bit.
// - Bits 3..0: Volume
// Between sustain and release is one byte that is not used and
// can be any value.
// The end of the release phase is encoded by a 0.
const char tt_InsFreqVolTable[] = {
%%INSFREQVOLTABLE%%
};

// =====================================================================
// Percussion instrument definitions (up to 15)
;
// Each percussion instrument is defined by:
// - tt_PercIndexes: The index of the first percussion frame as defined
//       in tt_PercFreqTable and tt_PercCtrlVolTable
// - tt_PercFreqTable: The AUDF frequency value
// - tt_PercCtrlVolTable: The AUDV volume and AUDC values
// =====================================================================

// Indexes into percussion definitions signifying the first frame for
// each percussion in tt_PercFreqTable.
// Caution: Values are stored with an implicit +1 modifier! To get the
// real index, subtract 1.
const char tt_PercIndexes[] = {
%%PERCINDEXES%%
}

// The AUDF frequency values for the percussion instruments.
// If the second to last value is negative (>=128), it means it's an
// "overlay" percussion, i.e. the player fetches the next instrument note
// immediately and starts it in the sustain phase next frame. (Needs
// TT_USE_OVERLAY)
const char tt_PercFreqTable[] = {
%%PERCFREQTABLE%%
};

// The AUDCx and AUDVx volume values for the percussion instruments.
// - Bits 7..4: AUDC value
// - Bits 3..0: AUDV value
// 0 means end of percussion data.
const char tt_PercCtrlVolTable[] = {
%%PERCCTRLVOLTABLE%%
};
        
// =====================================================================
// Track definition
// The track is defined by:
// - tt_PatternX (X=0, 1, ...): Pattern definitions
// - tt_PatternPtrLo/Hi: Pointers to the tt_PatternX tables, serving
//       as index values
// - tt_SequenceTable: The order in which the patterns should be played,
//       i.e. indexes into tt_PatternPtrLo/Hi. Contains the sequences
//       for all channels and sub-tracks. The variables
//       tt_cur_pat_index_c0/1 hold an index into tt_SequenceTable for
//       each channel.
//
// So tt_SequenceTable holds indexes into tt_PatternPtrLo/Hi, which
// in turn point to pattern definitions (tt_PatternX) in which the notes
// to play are specified.
// =====================================================================

// ---------------------------------------------------------------------
// Pattern definitions, one table per pattern. tt_cur_note_index_c0/1
// hold the index values into these tables for the current pattern
// played in channel 0 and 1.
//
// A pattern is a sequence of notes (one byte per note) ending with a 0.
// A note can be either:
// - Pause: Put melodic instrument into release. Must only follow a
//       melodic instrument.
// - Hold: Continue to play last note (or silence). Default "empty" note.
// - Slide (needs TT_USE_SLIDE): Adjust frequency of last melodic note
//       by -7..+7 and keep playing it
// - Play new note with melodic instrument
// - Play new note with percussion instrument
// - End of pattern
//
// A note is defined by:
// - Bits 7..5: 1-7 means play melodic instrument 1-7 with a new note
//       and frequency in bits 4..0. If bits 7..5 are 0, bits 4..0 are
//       defined as:
//       - 0: End of pattern
//       - [1..15]: Slide -7..+7 (needs TT_USE_SLIDE)
//       - 8: Hold
//       - 16: Pause
//       - [17..31]: Play percussion instrument 1..15
//
// The tracker must ensure that a pause only follows a melodic
// instrument or a hold/slide.
// ---------------------------------------------------------------------
const char TT_FREQ_MASK    = 0x1f; 
const char TT_INS_HOLD     = 8;
const char TT_INS_PAUSE    = 16;
const char TT_FIRST_PERC   = 17;

%%PATTERNDEFS%%

// Individual pattern speeds (needs TT_GLOBAL_SPEED = 0).
// Each byte encodes the speed of one pattern in the order
// of the tt_PatternPtr tables below.
// If TT_USE_FUNKTEMPO is 1, then the low nibble encodes
// the even speed and the high nibble the odd speed.
#ifndef TT_GLOBAL_SPEED
const char tt_PatternSpeeds[] = {
%%PATTERNSPEEDS%%
};
#endif

// ---------------------------------------------------------------------
// Pattern pointers look-up table.
// ---------------------------------------------------------------------
const char tt_PatternPtrLo[] = {
%%PATTERNPTRLO%%
};

const char tt_PatternPtrHi[] = {
%%PATTERNPTRHI%%       
}; 

// ---------------------------------------------------------------------
// Pattern sequence table. Each byte is an index into the
// tt_PatternPtrLo/Hi tables where the pointers to the pattern
// definitions can be found. When a pattern has been played completely,
// the next byte from this table is used to get the address of the next
// pattern to play. tt_cur_pat_index_c0/1 hold the current index values
// into this table for channels 0 and 1.
// If TT_USE_GOTO is used, a value >=128 denotes a goto to the pattern
// number encoded in bits 6..0 (i.e. value AND %01111111).
// ---------------------------------------------------------------------
const char tt_SequenceTable[] = {
        // ---------- Channel 0 ----------
%%SEQUENCECHANNEL0%%
,        
        // ---------- Channel 1 ----------
%%SEQUENCECHANNEL1%%
};

char tia_tracker_init()
{
    tt_timer = 0;
    tt_cur_note_index_c0 = 0;
    tt_cur_note_index_c1 = 0;
    tt_cur_pat_index_c0 = %%C0INIT%%;
    tt_cur_pat_index_c1 = %%C1INIT%%;
}
