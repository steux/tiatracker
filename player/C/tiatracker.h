// TIATracker music player
// Copyright 2016 Andre "Kylearan" Wichmann
// Website: https://bitbucket.org/kylearan/tiatracker
// Email: andre.wichmann@gmx.de
;
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
;
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

#ifndef __TIA_TRACKER_H__
#define __TIA_TRACKER_H__

// =====================================================================
// Flags
// =====================================================================

// 1: Global song speed, 0: Each pattern has individual speed
const char TT_GLOBAL_SPEED         = %%GLOBALSPEED%%
// duration (number of TV frames) of a note
const char TT_SPEED                = %%EVENSPEED%%
// duration of odd frames (needs TT_USE_FUNKTEMPO)
const char TT_ODD_SPEED            = %%ODDSPEED%%

// 1: Overlay percussion, +40 bytes
const char TT_USE_OVERLAY          = %%USEOVERLAY%%
// 1: Melodic instrument slide, +9 bytes
const char TT_USE_SLIDE            = %%USESLIDE%%
// 1: Goto pattern, +8 bytes
const char TT_USE_GOTO             = %%USEGOTO%%
// 1: Odd/even rows have different SPEED values, +7 bytes
const char TT_USE_FUNKTEMPO        = %%USEFUNKTEMPO%%
// If the very first notes played on each channel are not PAUSE, HOLD or
// SLIDE, i.e. if they start with an instrument or percussion, then set
// this flag to 0 to save 2 bytes.
// 0: +2 bytes
const char TT_STARTS_WITH_NOTES    = %%STARTSWITHNOTES%%

// =====================================================================
// Permanent variables. These are states needed by the player.
// =====================================================================
char tt_timer                ;    // current music timer value
char tt_cur_pat_index_c0     ;    // current pattern index into tt_SequenceTable
char tt_cur_pat_index_c1     ;
char tt_cur_note_index_c0    ;    // note index into current pattern
char tt_cur_note_index_c1    ;
char tt_envelope_index_c0    ;    // index into ADSR envelope
char tt_envelope_index_c1    ;
char tt_cur_ins_c0           ;    // current instrument
char tt_cur_ins_c1           ;


// =====================================================================
// Temporary variables. These will be overwritten during a call to the
// player routine, but can be used between calls for other things.
// =====================================================================
char *tt_ptr;

}
#endif // __TIA_TRACKER_H__

