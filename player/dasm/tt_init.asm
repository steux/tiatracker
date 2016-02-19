; =====================================================================
; Initialize music.
; Set tt_cur_pat_index_c0/1 to the indexes of the first patterns from
; tt_SequenceTable for each channel.
; Set tt_timer and tt_cur_note_index_c0/1 to 0.
; All other variables can start with any value.
; =====================================================================
        lda #TT_C0_INTRO
        sta tt_cur_pat_index_c0
        lda #TT_C1_INTRO
        sta tt_cur_pat_index_c1
        ; the rest should be 0 already from startup code. If not,
        ; set them manually to 0 here.

