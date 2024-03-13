procedure myDLI(); assembler; interrupt;
Asm
COLPF0  = $D016  ; (708)
COLPF1  = $D017  ; (709)
COLPF2  = $D018  ; (710)
COLPF3  = $D019  ; (711)
COLPF4  = $D01A  ; (712)
WSYNC   = $D40A

  .macro m@setVDLI newVDLI
    lda #<:newVDLI
    sta DLIV
    lda #>:newVDLI
    sta DLIV+1
  .endm

dli1: // workarea
  pha

  m@setVDLI DLI3

  lda #$E0
  sta wsync
  sta wsync
  sta COLPF4

  pla
  rti

dli2: // timeline
  pha
  lda #$00
  sta wsync
  // sta wsync
  sta COLPF4

  m@setVDLI DLI3

  pla
  rti

dli3: // controls
  pha:txa:pha
  lda #$E4
  ldx #$E0
  sta wsync
  // sta wsync
  sta COLPF4
  stx COLPF1
  m@setVDLI DLI4

  pla:tax:pla
  rti

dli4: // status
  pha:txa:pha
  lda #$04
  ldx #$0f
  sta wsync
  sta COLPF4
  stx COLPF1
  m@setVDLI DLI5

  pla:tax:pla
  rti

dli5:
  pha
  lda 712
  sta wsync
  sta COLPF4

  m@setVDLI DLI1

  pla
  rti

End;
