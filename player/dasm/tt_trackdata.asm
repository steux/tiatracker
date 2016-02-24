; =====================================================================
; TIATracker melodic and percussion instruments, patterns and sequencer
; data.
; =====================================================================
tt_TrackDataStart:

; =====================================================================
; Melodic instrument definitions (up to 7). tt_envelope_index_c0/1 hold
; the index values into these tables for the current instruments played
; in channel 0 and 1.
; 
; Each instrument is defined by:
; - tt_InsCtrlTable: the AUDC value
; - tt_InsADIndexes: the index of the start of the ADSR envelope as
;       defined in tt_InsFreqVolTable
; - tt_InsSustainIndexes: the index of the start of the Sustain phase
;       of the envelope
; - tt_InsReleaseIndexes: the index of the start of the Release phase
; - tt_InsFreqVolTable: The AUDF frequency and AUDV volume values of
;       the envelope
; =====================================================================

TT_INS_LEAD     = 2<<5

; Instrument master CTRL values
tt_InsCtrlTable:
        dc.b 1, 1

; Instrument Attack/Decay start indexes into ADSR tables.
tt_InsADIndexes:
        dc.b 0, 10

; Instrument Sustain start indexes into ADSR tables
tt_InsSustainIndexes:
        dc.b 2, 18

; Instrument Release start indexes into ADSR tables
; Caution: Values are stored with an implicit -1 modifier! To get the
; real index, add 1.
tt_InsReleaseIndexes:
        dc.b 5, 22

; AUDVx and AUDFx ADSR envelope values.
; Each byte encodes the frequency and volume:
; - Bits 7..4: Freqency modifier for the current note ([-8..7]),
;       8 means no change. Bit 7 is the sign bit.
; - Bits 3..0: Volume
; Between sustain and release is one byte that is not used and
; can be any value.
; The end of the release phase is encoded by a 0.
tt_InsFreqVolTable:
        ; Test Lead
        ;   |   A/D   | Sustain      | -- | Release      | End |
        ;      0    1    2    3    4    5    6    7    8    9
        dc.b $11, $22, $33, $44, $55, $00, $66, $77, $88, $00
        
        ; Lead #1
        ;   |   Attack & Decay                      | Sustain           | -- | Release           | End |
        ;      0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17
        dc.b $81, $71, $62, $53, $84, $75, $66, $57, $88, $78, $68, $58, $aa, $86, $74, $62, $50, $00
        

; =====================================================================
; Percussion instrument definitions (up to 15)
;
; Each percussion instrument is defined by:
; - tt_PercIndexes: The index of the first percussion frame as defined
;       in tt_PercFreqTable and tt_PercCtrlVolTable
; - tt_PercFreqTable: The AUDF frequency value
; - tt_PercCtrlVolTable: The AUDV volume and AUDC values
; =====================================================================

TT_PERC_SNARE           = 17
TT_PERC_CLAP            = 18
TT_PERC_BASSDRUM        = 19

; Indexes into percussion definitions signifying the first frame for
; each percussion in tt_PercFreqTable.
; Caution: Values are stored with an implicit +1 modifier! To get the
; real index, subtract 1.
tt_PercIndexes:
        dc.b 1, 8, 12

; The AUDF frequency values for the percussion instruments.
; If the second to last value is negative (>=128), it means it's an
; "overlay" percussion, i.e. the player fetches the next instrument note
; immediately and starts it in the sustain phase next frame. (Needs
; TT_USE_OVERLAY)
tt_PercFreqTable:
        ; Snare
        dc.b 14, 14, 1, 1, 1, 0, 0
        ; Clap
        dc.b 5, 5, 5, 0
        ; Bassdrum
        dc.b 8, 8, 16, 16, 20, 128+24, 0

; The AUDCx and AUDVx volume values for the percussion instruments.
; - Bits 7..4: AUDC value
; - Bits 3..0: AUDV value
; 0 means end of percussion data.
tt_PercCtrlVolTable:
        ; Snare
        dc.b $fc, $fc, $84, $84, $84, $84, $00
        ; Clap
        dc.b $87, $87, $80, $00
        ; Bassdrum
        dc.b $aa, $aa, $aa, $aa, $aa, $0a, $00

        
; =====================================================================
; Track definition
; The track is defined by:
; - tt_PatternX (X=0, 1, ...): Pattern definitions
; - tt_PatternPtrLo/Hi: Pointers to the tt_PatternX tables, serving
;       as index values
; - tt_SequenceTable: The order in which the patterns should be played,
;       i.e. indexes into tt_PatternPtrLo/Hi. Contains the sequences
;       for all channels and sub-tracks. The variables
;       tt_cur_pat_index_c0/1 hold an index into tt_SequenceTable for
;       each channel.
;
; So tt_SequenceTable holds indexes into tt_PatternPtrLo/Hi, which
; in turn point to pattern definitions (tt_PatternX) in which the notes
; to play are specified.
; =====================================================================

; ---------------------------------------------------------------------
; Pattern definitions, one table per pattern. tt_cur_note_index_c0/1
; hold the index values into these tables for the current pattern
; played in channel 0 and 1.
;
; A pattern is a sequence of notes (one byte per note) ending with a 0.
; A note can be either:
; - Pause: Put melodic instrument into release. Must only follow a
;       melodic instrument.
; - Hold: Continue to play last note (or silence). Default "empty" note.
; - Slide (needs TT_USE_SLIDE): Adjust frequency of last melodic note
;       by -7..+7 and keep playing it
; - Play new note with melodic instrument
; - Play new note with percussion instrument
; - End of pattern
;
; A note is defined by:
; - Bits 7..5: 1-7 means play melodic instrument 1-7 with a new note
;       and frequency in bits 4..0. If bits 7..5 are 0, bits 4..0 are
;       defined as:
;       - 0: End of pattern
;       - [1..15]: Slide -7..+7 (needs TT_USE_SLIDE)
;       - 8: Hold
;       - 16: Pause
;       - [17..31]: Play percussion instrument 1..15
;
; The tracker must ensure that a pause only follows a melodic
; instrument or a hold/slide.
; ---------------------------------------------------------------------
TT_FREQ_MASK    = %00011111
TT_INS_HOLD     = 8
TT_INS_PAUSE    = 16
TT_FIRST_PERC   = 17

tt_Pattern0:
        dc.b TT_INS_LEAD|1, TT_INS_HOLD, TT_INS_HOLD, TT_INS_PAUSE
        dc.b TT_INS_LEAD|3, TT_INS_HOLD, TT_INS_HOLD, TT_INS_PAUSE
        dc.b TT_INS_LEAD|5, TT_INS_HOLD, TT_INS_HOLD, TT_INS_PAUSE
        dc.b 0

tt_Pattern1:
        dc.b TT_INS_HOLD
        dc.b 0
        
tt_Pattern2:
        dc.b TT_PERC_BASSDRUM, TT_INS_LEAD|20, TT_PERC_BASSDRUM, TT_INS_LEAD|20, TT_PERC_BASSDRUM, TT_INS_LEAD|20, TT_PERC_BASSDRUM, TT_INS_LEAD|20
        dc.b 0


; ---------------------------------------------------------------------
; Pattern pointers look-up table.
; ---------------------------------------------------------------------
tt_PatternPtrLo:
        dc.b <tt_Pattern0
        dc.b <tt_Pattern1
        dc.b <tt_Pattern2
tt_PatternPtrHi:
        dc.b >tt_Pattern0
        dc.b >tt_Pattern1
        dc.b >tt_Pattern2


; ---------------------------------------------------------------------
; Pattern sequence table. Each byte is an index into the
; tt_PatternPtrLo/Hi tables where the pointers to the pattern
; definitions can be found. When a pattern has been played completely,
; the next byte from this table is used to get the address of the next
; pattern to play. tt_cur_pat_index_c0/1 hold the current index values
; into this table for channels 0 and 1.
; If TT_USE_GOTO is used, a value >=128 denotes a goto to the pattern
; number encoded in bits 6..0 (i.e. value AND %01111111).
; ---------------------------------------------------------------------
tt_SequenceTable:
        
        ; ---------- Channel 0 ----------
TT_C0_INTRO     = *-tt_SequenceTable

TT_C0_LOOP      = *-tt_SequenceTable
        dc.b 0, 0
        ; Now loop
        dc.b $80+TT_C0_LOOP
        
        
        ; ---------- Channel 1 ----------
TT_C1_INTRO     = *-tt_SequenceTable

TT_C1_LOOP      = *-tt_SequenceTable
        dc.b 1, 1
        ; Now loop
        dc.b $80+TT_C1_LOOP

        echo "Track size: ", *-tt_TrackDataStart
