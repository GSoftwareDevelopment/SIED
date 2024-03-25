{$I 'graph.var.inc'}

procedure putImage(adr:Pointer; x,y,width,height:Byte); Register; assembler; Asm icl 'asm/put-image.a65' End;
procedure putSprite(adr:Pointer; x,y,width,height:Byte); Register; assembler; Asm icl 'asm/put-sprite-or.a65' End;
procedure putSpriteXOR(adr:Pointer; x,y,width,height:Byte); Register; assembler; Asm icl 'asm/put-sprite-xor.a65' End;
procedure putChar(x,y:Shortint; ch:Char); Register; assembler; Asm icl 'asm/put-char.a65' End;
procedure putCharC(x,y:Shortint; ch:Char); Register; assembler; Asm icl 'asm/put-charc.a65' End;

procedure putText(x,y:Shortint; s:PString); Register; overload; assembler; Asm icl 'asm/put-text.a65' End;
procedure putText(adr:pointer; s:PString); overload; assembler;
asm
  lda adr
  sta scr
  lda adr+1
  sta scr+1
  lda s
  sta MAIN.putText.@03018986018300018300.s
  lda s+1
  sta MAIN.putText.@03018986018300018300.s+1
  jsr MAIN.putText.@03018986018300018300.putText
end;

procedure putTextC(x,y:Shortint; s:PString); Register; overload; assembler; Asm icl 'asm/put-textc.a65' End;
procedure putTextC(adr:pointer; s:PString); overload; assembler;
asm
  lda adr
  sta scr
  lda adr+1
  sta scr+1
  lda s
  sta MAIN.putTextC.@03018986018300018300.s
  lda s+1
  sta MAIN.putTextC.@03018986018300018300.s+1
  jsr MAIN.putTextC.@03018986018300018300.putText
end;

procedure invert(x,y,w,h:Shortint); Register; assembler; Asm icl 'asm/invert.a65' End;
procedure blank(x,y,w,h:Shortint); Register; assembler; Asm icl 'asm/blank.a65' End;

procedure SetScreenWidth(nSW:Byte); Register; assembler;
Asm
  sta MAIN.PUTIMAGE._LNWIDTH
  sta MAIN.PUTSPRITE._LNWIDTH1
  sta MAIN.PUTSPRITE._LNWIDTH2
  sta MAIN.BLANK._LNWIDTH1
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