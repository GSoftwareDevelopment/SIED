{$define BASICOFF}
{$define ROMOFF}
{$LIBRARYPATH 'bin/'}

library trail:$4000;

const
{$I 'data/data-mem.inc'}

{$I 'core/pmg.var.inc'}

{$I 'core/assets.h.inc'}
{$I 'core/keyboard.h.inc'}
{$I 'core/graph.h.inc'}
{$I 'core/interface.h.inc'}
{$I 'core/controls.h.inc'}
{$I 'core/utils.h.inc'}

const
  TR_JUMP = $80;
  TR_STOP = $88;
  TR_KILL = $8F;

const
  PATHNAMESIZE  = 28;

var
  pathListPtr:Array[0..MAXPATHDEFINITIONS] of pointer       absolute PATHLISTV_ADDR;
  pathNamePtr:Array[0..MAXPATHDEFINITIONS-1] of pointer     absolute PATHNAMEV_ADDR;
  pathStartEdge:Array[0..MAXPATHDEFINITIONS-1] of byte      absolute PATHSTARTX_ADDR;
  pathStartEdgeShift:Array[0..MAXPATHDEFINITIONS-1] of byte absolute PATHSTARTY_ADDR;
  pathNameSearch:String[PATHNAMESIZE];

procedure showTrailEditor(); forward;
{$I 'trail-selector.inc'}
{$I 'trail-editor.inc'}

//

var
  moduleInitialized:Byte absolute $62;
  [volatile] PORTB:Byte absolute $D301;

begin
  asm
    lda PORTB
    pha
    and #$FE
    sta PORTB
  end;
  if (moduleInitialized and $2=0) then
  begin
    fillchar(pointer(PATHLISTV_ADDR),$1000,$00); // clear pathListPtr, pathNamePtr, pathNames
    for i:=0 to MAXPATHDEFINITIONS-1 do
    begin
      pathStartEdge[i]:=EDGE_RIGHT;
      pathStartEdgeShift[i]:=0;
    end;
    pathListPtr[0]:=pointer($A000);
    moduleInitialized:=moduleInitialized or $2;
  end;
  redrawList:=true; curAction:=-1;
  // fillchar(Pointer(PMG_ADDR+$180+24),48,%01100000);
  fillchar(Pointer(PMG_ADDR+$300+23),50,$FF);
  fillchar(Pointer(PMG_ADDR+$380+23),50,$3F);
  // fillchar(Pointer(PMG_ADDR+$180+24+65),7,$02);
  poke(PMG_ADDR+$180+24+65,$02);
  poke(PMG_ADDR+$180+24+66,$02);
  poke(PMG_ADDR+$180+24+67,$00);
  poke(PMG_ADDR+$180+24+68,$00);
  poke(PMG_ADDR+$180+24+69,$00);
  poke(PMG_ADDR+$180+24+70,$02);
  poke(PMG_ADDR+$180+24+71,$02);
  PCOL[2]:=BASE_COLOR+$02; PCOL[3]:=BASE_COLOR+$02;
  SIZEP[2]:=%11; SIZEP[3]:=%11; SIZEM:=%00000000;
  HPOSP[2]:=44; // icon tray
  clearAllShortcutsKey();
  showTrailSelector();
  asm
    pla
    sta PORTB
  end;
end.