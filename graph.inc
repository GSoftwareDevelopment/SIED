var
  DLIV:Pointer absolute $200;
  [volatile] NMIEN:Byte absolute $D40E;
  // SDMACTL:Byte absolute $22F;
  SDLST:Pointer absolute $230;
  GTIACS:Byte absolute $026F;
  PFCOL0:Byte absolute 708;
  PFCOL1:Byte absolute 709;
  PFCOL2:Byte absolute 710;
  PFCOL3:Byte absolute 711;
  PFCOL4:Byte absolute 712;
  activePage:Byte absolute zp_activePage;
  scr:Pointer absolute zp_scr;
  YSCR:Array[0..127] of Pointer absolute SCREEN_LINES_ADDR;    // 0-55 first page 56-111 secound page
  AFONT : Array[0..127] of Word absolute FONTS_ADDR;
  AFONTC : Array[0..63] of Word absolute FONTS_ADDR+$100;

procedure putImage(adr:Pointer; x,y,width,height:Byte); Register; assembler; Asm icl 'asm/put-image.a65' End;
procedure putSprite(adr:Pointer; x,y:shortint; width,height:shortint); register; keep; assembler; Asm icl 'asm/put-sprite-or.a65' End;
procedure putSpriteXOR(adr:Pointer; x,y,width,height:Byte); Register; assembler; Asm icl 'asm/put-sprite-xor.a65' End;
procedure putText(x,y:Shortint; s:PString); Register; assembler; Asm icl 'asm/put-text.a65' End;
procedure putTextC(x,y:Shortint; s:PString); Register; assembler; Asm icl 'asm/put-textc.a65' End;

procedure invert(x,y,w,h:Shortint); Register; assembler; Asm icl 'asm/invert.a65' End;
procedure blank(x,y,w,h:Shortint); Register; assembler; Asm icl 'asm/blank.a65' End;

procedure SetScreenWidth(nSW:Byte); Register; inline; assembler;
Asm
  sta MAIN.PUTIMAGE._LNWIDTH
  sta MAIN.PUTSPRITE._LNWIDTH1
  sta MAIN.PUTSPRITE._LNWIDTH2
End;

procedure switchActivePage(); inline; assembler;
Asm
  lda activePage
  eor #1
  sta activePage
  lda #0
  sta 77
End;

procedure setVisiblePage(visiblePage:Byte); Register; Assembler;
Asm
  // lda visiblePage
  bne setPage1

setPage0:
  sei
  lda #<DLIST0_ADDR        ; set visible page 0
  ldy #>DLIST0_ADDR
  sta SDLST
  sty SDLST+1
  cli
  rts

setPage1:
  sei
  lda #<DLIST1_ADDR        ; set visible page 1
  ldy #>DLIST1_ADDR
  sta SDLST
  sty SDLST+1
  cli
End;

procedure clearPage(page:Byte); Register; Assembler;
Asm
  // lda page
  bne setPage1

setPage0:
  ldy #>SCREEN_ADDR
  bne clear

setPage1:
  ldy #>(SCREEN_ADDR+$400)

clear:                      ; clear active page
  lda #0
  sta _pp0
  sty _pp0+1
  iny
  sta _pp1
  sty _pp1+1
  iny
  sta _pp2
  sty _pp2+1
  iny
  sta _pp3
  sty _pp3+1

  tay
lclr:
  sta _pp0:$8000,y
  sta _pp1:$8100,y
  sta _pp2:$8200,y
  sta _pp3:$8300,y
  dey
  bne lclr

End;

procedure wait(f:Byte); Register; assembler;
Asm
  // Accu have value od variable 'f'
  // lda $14
  // add f
  add $14
loop:
  cmp $14
  bne *-2
End;