{$define BASICOFF}
{$define ROMOFF}
{$define NOROMFONT}
{$UNITPATH './'}
{$LIBRARYPATH './core/bin/'}
{$LIBRARYPATH './bin/'}

program Editor;
uses cio;

const
{$I 'data/data-mem.inc'}
{$R 'data/data.rc'}
{$R 'libs.rc'}

{$I 'core/pmg.var.inc'}

{$I 'core/assets.h.inc'}
{$I 'core/keyboard.h.inc'}
{$I 'core/graph.h.inc'}
{$I 'core/interface.h.inc'}
{$I 'core/controls.h.inc'}

{$I 'core/dlist-interrupt.inc'}
{$I 'core/utils.h.inc'}
{$I 'core/cursor.h.inc'}

var
  i:Shortint absolute $3e;
  tm:Byte absolute $14;

{$I 'module.inc'}

procedure myVBL(); interrupt; keep; assembler;
Asm
  lda #0
  sta 77

  lda #<MAIN.myDLI
  sta DLIV
  lda #>MAIN.myDLI
  sta DLIV+1

  icl 'core/asm/cursor.a65'
exVBL:
  jmp xitvbv
End;

procedure init();
var
  KEYDEFP:Pointer absolute $79;

begin
  SDMACTL:=0; // turn off screen
  NMIEN:=$00;
  // graphics init
  for i:=0 to 55 do YSCR[i]:=Pointer(SCREEN_ADDR+i*$10);
  for i:=0 to 47 do YSCR[56+i]:=Pointer(EDITOR_ADDR+i*20);
  for i:=0 to 23 do YSCR[56+48+i]:=Pointer(EDITOR_ADDR+(20*48)+i*40);
  fillchar(Pointer(PMG_ADDR+$180),$E00,0); // clear PMG, SCREEN & EDITOR area at once
  ActivePage:=1;
  initCursor(@myVBL);
  Asm sei; End;
  setIntVec(iDLI,@myDLI);
  SDLST:=pointer(DLIST_ADDR);
  PFCOL0:=$EA; PFCOL1:=$00; PFCOL2:=$0F; PFCOL4:=$e0;
  // keyboard init
  KEYDEFP:=Pointer(SCAN2ASC_ADDR);
  KRPDEL:=10; KEYREP:=2;
  SHFLOK:=$00; // Shift & Control Lock are disabled
  Asm cli; End;
  NMIEN:=$C0;
  wait(1);
  initShortcutKeyboard();
  // interface init
  initInterface();
End;

begin
  init();
  initModules();
  runInterface();
End.
