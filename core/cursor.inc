{$i 'cursor.var.inc'}
{$I 'pmg.var.inc'}

procedure setCursor(spr:Pointer); Register; assembler;
Asm
  lda spr
  sta main.CURSORSPR
  lda spr+1
  sta main.CURSORSPR+1
  lda #1
  sta mRefresh
  lda $14
  cmp $14
  beq *-2
End;

procedure setPivot(x,y:Shortint); Register; assembler;
Asm
  lda x
  sta main.mpivotX
  lda y
  sta main.mpivotY
End;

procedure initCursor(VBLPtr:Pointer);
begin
  PMBASE:=HI(PMG_ADDR);
  PMCTL:=3;
  GPRIOR:=%00000010;
  SDMACTL:=%00101010;
  // fillchar(Pointer(PMG_ADDR+$200),256,0);
  PCOL[0]:=$0f; PCOL[1]:=$00;
  SetIntVec(iVBL,VBLPtr);
  mRefresh:=true;
  mAllowKeys:=true;
  _mx:=128; _my:=64;
  setCursor(_ARROW);
  setPivot(0,0);
End;