        processor 6502
        include vcs.h
        
; TV format switches
PAL             = 1
NTSC            = 0

        IF PAL
TIM_VBLANK      = 43
TIM_OVERSCAN    = 36
TIM_KERNEL      = 19
        ELSE
TIM_VBLANK      = 45
TIM_OVERSCAN    = 38
TIM_KERNEL      = 15
        ENDIF


; =====================================================================
; Variables
; =====================================================================

        SEG.U   variables
        ORG     $80

        include "tt_variables.asm"

; test
player_time_max         ds 1


; =====================================================================
; Start of code
; =====================================================================

        SEG     Bank0
        ORG     $f000

Start   SUBROUTINE

        ; Clear zeropage        
        cld
        ldx #0
        txa
.clearLoop:
        dex
        tsx
        pha
        bne .clearLoop

        include "tt_init.asm"

        
; =====================================================================
; MAIN LOOP
; =====================================================================

MainLoop:

; ---------------------------------------------------------------------
; Overscan
; ---------------------------------------------------------------------

Overscan        SUBROUTINE

        sta WSYNC
        lda #2
        sta VBLANK
        lda #TIM_OVERSCAN
        sta TIM64T

        ; Do overscan stuff

.waitForIntim
        lda INTIM
        bne .waitForIntim

; ---------------------------------------------------------------------
; VBlank
; ---------------------------------------------------------------------

VBlank  SUBROUTINE

        lda #%1110
.vsyncLoop:
        sta WSYNC
        sta VSYNC
        lsr
        bne .vsyncLoop
        lda #2
        sta VBLANK
        lda #TIM_VBLANK
        sta TIM64T

        ; Do VBlank stuff
        include "tt_player.asm"
        
        ; Measure player worst case timing
        lda #TIM_VBLANK
        sec
        sbc INTIM
        cmp player_time_max
        bcc .noNewMax
        sta player_time_max
.noNewMax:


.waitForVBlank:
        lda INTIM
        bne .waitForVBlank
        sta WSYNC
        sta VBLANK


; ---------------------------------------------------------------------
; Kernel
; ---------------------------------------------------------------------

Kernel  SUBROUTINE
        lda #TIM_KERNEL
        sta T1024T

        ; Do kernel stuff

.waitForIntim:
        lda INTIM
        bne .waitForIntim
        
        jmp MainLoop


; =====================================================================
; Data
; =====================================================================

        include "tt_trackdata.asm"


; =====================================================================
; Vectors
; =====================================================================

        echo "ROM left: ", ($fffc - *)

        ORG             $fffc
        .word   Start
        .word   Start
