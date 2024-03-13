{$i 'cursor.var.inc'}
{$I 'pmg.var.inc'}

procedure setCursorShape(spr:Pointer); Register; assembler;
Asm
  lda spr
  sta main.CURSORSPR
  lda spr+1
  sta main.CURSORSPR+1
  lda #1
  sta mRefresh
End;

procedure setCursorShapeAnchor(x,y:Shortint); Register; assembler;
Asm
  lda x
  sta main.mpivotX
  lda y
  sta main.mpivotY
  lda #1
  sta mRefresh
End;

procedure setCursor(cur:Pointer); Register; Assembler;
asm
  ldy #0
  lda (cur),y
  sta main.mpivotX
  iny
  lda (cur),y
  sta main.mpivotY

  lda cur
  add #2
  sta main.CURSORSPR
  lda cur+1
  adc #0
  sta main.CURSORSPR+1

  lda #1
  sta mRefresh
end;

procedure setCursorPos(x,y:Byte); Register;
begin
  mx:=x shr 2; my:=y;
  _mx:=48+x;
  _my:=24+y;
  mdx:=0; mdy:=0; mRefresh:=true;
end;

procedure initCursor(VBLPtr:Pointer);
begin
  PCOL[0]:=$0f; PCOL[1]:=$00;
  SetIntVec(iVBL,VBLPtr);
  mRefresh:=true;
  _mx:=128; _my:=64;
  setCursor(_ARROW);
  PMBASE:=HI(PMG_ADDR);
  PMCTL:=3;
  GPRIOR:=%00000010;
  SDMACTL:=%00101010;
End;