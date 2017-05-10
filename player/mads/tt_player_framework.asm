; TIATracker music player (MADS format)
; Copyright 2016 Andre "Kylearan" Wichmann
; Website: https://bitbucket.org/kylearan/tiatracker
; Email: andre.wichmann@gmx.de
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
;   http://www.apache.org/licenses/LICENSE-2.0
;
; Unless required by applicable law or agreed to in writing, software
; distributed under the License is distributed on an "AS IS" BASIS,
; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; See the License for the specific language governing permissions and
; limitations under the License.

; Song author: %%AUTHOR%%
; Song name: %%NAME%%

; @com.wudsn.ide.asm.hardware=ATARI2600

        icl "VCS.asm"
        
; TV format switches
PAL             = %%PAL%%
NTSC            = %%NTSC%%

        .if PAL
TIM_VBLANK      = 43
TIM_OVERSCAN    = 36
TIM_KERNEL      = 19
        .else
TIM_VBLANK      = 45
TIM_OVERSCAN    = 38
TIM_KERNEL      = 15
        .endif


; =====================================================================
; Variables
; =====================================================================

        opt h-f-
        org     $80

        icl "%%FILENAME%%_variables.asm"

; test
player_time_max         .ds 1


; =====================================================================
; Start of code
; =====================================================================

        opt f+
        org     $f000

        .proc Start

        ; Clear zeropage        
        cld
        ldx #0
        txa
clearLoop:
        dex
        txs
        pha
        bne clearLoop

        icl "%%FILENAME%%_init.asm"

        
; =====================================================================
; MAIN LOOP
; =====================================================================

MainLoop:

; ---------------------------------------------------------------------
; Overscan
; ---------------------------------------------------------------------

        .proc Overscan       

        sta WSYNC
        lda #2
        sta VBLANK
        lda #TIM_OVERSCAN
        sta TIM64T

        ; Do overscan stuff

waitForIntim
        lda INTIM
        bne waitForIntim
        .endp

; ---------------------------------------------------------------------
; VBlank
; ---------------------------------------------------------------------

        .proc VBlank

        lda #%1110
vsyncLoop:
        sta WSYNC
        sta VSYNC
        lsr
        bne vsyncLoop
        lda #2
        sta VBLANK
        lda #TIM_VBLANK
        sta TIM64T

        ; Do VBlank stuff
        icl "%%FILENAME%%_player.asm"
        
        ; Measure player worst case timing
        lda #TIM_VBLANK
        sec
        sbc INTIM
        cmp player_time_max
        bcc noNewMax
        sta player_time_max
noNewMax:

waitForVBlank:
        lda INTIM
        bne waitForVBlank
        sta WSYNC
        sta VBLANK
        .endp

; ---------------------------------------------------------------------
; Kernel
; ---------------------------------------------------------------------

        .proc Kernel
        lda #TIM_KERNEL
        sta T1024T

        ; Do kernel stuff

waitForIntim:
        lda INTIM
        bne waitForIntim
        .endp

        jmp MainLoop
        .endp

; =====================================================================
; Data
; =====================================================================

        icl "%%FILENAME%%_trackdata.asm"


; =====================================================================
; Vectors
; =====================================================================

        .echo "ROM left: ", ($fffc - *)

        org $fffc
        .word   Start
        .word   Start
