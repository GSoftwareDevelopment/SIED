{$I 'graph-var.inc'}

procedure putImage(adr:Pointer; x,y,width,height:Byte); Register; assembler; Asm icl 'asm/put-image.a65' End;
procedure putSprite(adr:Pointer; x,y:shortint; width,height:shortint); register; keep; assembler; Asm icl 'asm/put-sprite-or.a65' End;
procedure putSpriteXOR(adr:Pointer; x,y,width,height:Byte); Register; assembler; Asm icl 'asm/put-sprite-xor.a65' End;
procedure putText(x,y:Shortint; s:PString); Register; assembler; Asm icl 'asm/put-text.a65' End;
procedure putTextC(x,y:Shortint; s:PString); Register; assembler; Asm icl 'asm/put-textc.a65' End;

procedure invert(x,y,w,h:Shortint); Register; assembler; Asm icl 'asm/invert.a65' End;
procedure blank(x,y,w,h:Shortint); Register; assembler; Asm icl 'asm/blank.a65' End;

procedure SetScreenWidth(nSW:Byte); Register; assembler;
Asm
  sta MAIN.PUTIMAGE._LNWIDTH
  sta MAIN.PUTSPRITE._LNWIDTH1
  sta MAIN.PUTSPRITE._LNWIDTH2
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