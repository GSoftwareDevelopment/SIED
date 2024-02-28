unit cursor;

interface
const
  _ARROW:Array of Byte = [
    // $80, $e0, $f8, $f0, $e0, $c0, $80
    $10, $18, $3c, $3e, $78, $60, $80
  ];
  _WAIT:Array of Byte = [
    $ba, $7c, $f6, $ee, $6c, $ba, $44
  ];
  _CROSS:Array of Byte = [
    $10, $10, $00, $c6, $00, $10, $10
  ];

  MBUT_NONE   = 0;
  MBUT_LEFT   = 1;

var
    mRefresh:Boolean  absolute $5c;
  mAllowKeys:Boolean  absolute $5d;
         _mx:Byte     absolute $52;
         _my:Byte     absolute $53;
          mx:Shortint absolute $55;
          my:Shortint absolute $54;
         mdx:Shortint absolute $5b;
         mdy:Shortint absolute $5a;
       stick:Byte     absolute $278;
     mbutton:Byte     absolute $4a;
    ombutton:Byte     absolute $4b;

procedure setCursor(spr:Pointer); Register; assembler;
procedure setPivot(x,y:Shortint); Register; assembler;
procedure initCursor();

implementation
const
{$I 'data-mem.inc'}
{$I 'pmg.inc'}

procedure setCursor(spr:Pointer); Register; assembler;
Asm
  lda spr
  sta main.cursor.myVBL.CURSORSPR
  lda spr+1
  sta main.cursor.myVBL.CURSORSPR+1
  lda #1
  sta mRefresh
  lda $14
  cmp $14
  beq *-2
End;

procedure setPivot(x,y:Shortint); Register; assembler;
Asm
  lda x
  sta main.cursor.myVBL.mpivotX
  lda y
  sta main.cursor.myVBL.mpivotY
End;

procedure myVBL(); interrupt; keep; assembler;
Asm
  lda #0
  sta 77

    // lda #<MAIN.cursor.myDLI
    // sta DLIV
    // lda #>MAIN.cursor.myDLI
    // sta DLIV+1

  icl 'asm/cursor.a65'
exVBL:
  jmp xitvbv
End;

procedure initCursor();
begin
  PMBASE:=HI(PMG_ADDR);
  PMCTL:=3;
  GPRIOR:=%00000010;
  SDMACTL:=%00101010;
  // fillchar(Pointer(PMG_ADDR+$200),256,0);
  PCOL[0]:=$0f; PCOL[1]:=$00;
  SetIntVec(iVBL,@myVBL);
  mRefresh:=true;
  mAllowKeys:=true;
  _mx:=128; _my:=64;
  setCursor(@_ARROW);
  setPivot(0,0);
End;

end.