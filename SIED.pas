{$define BASICOFF}
{$define ROMOFF}
{$define NOROMFONT}
{$UNITPATH './'}
{$LIBRARYPATH './core/bin/'}

program Editor;
uses cio,cursor;

const
{$I 'data-mem.inc'}
{$I 'scripts-mem.inc'}
{$I 'display-list.inc'}

{$R 'data.rc'}
{$I 'core/assets.h'}

{$I 'keyboard.inc'}
{$I 'utils.inc'}
{$I 'pmg.inc'}

{$I 'core/graph.h'}
{$I 'core/interface.h'}
{$I 'core/controls.h'}

var
  i:Shortint absolute $3e;
  tm:Byte absolute $14;

  curModule:Byte = -1;

procedure setModule(cm:Shortint); Forward;
{$I 'about.inc'}
{$I 'module-disk.inc'}
{$I 'module-path.inc'}
{$I 'module-scenario.inc'}
{$I 'module.inc'}

procedure initEditor();
var
  KEYDEFP:Pointer absolute $79;

begin
  KEYDEFP:=Pointer(SCAN2ASC_ADDR);
  for i:=0 to 55 do YSCR[i]:=Pointer(SCREEN_ADDR+i*$10);
  for i:=0 to 47 do YSCR[56+i]:=Pointer(EDITOR_ADDR+i*20);
  for i:=0 to 23 do YSCR[56+48+i]:=Pointer(EDITOR_ADDR+(20*48)+i*40);
  fillchar(Pointer(PMG_ADDR),$1000,0);
  fillchar(Pointer(PMG_ADDR+$300+23),50,$FF);
  ActivePage:=1;
  SDMACTL:=0; // turn off screen
  Asm
    lda $14
    cmp $14
    beq *-2
  End;
  Asm sei; End;
  setIntVec(iDLI,@myDLI);
  NMIEN:=$C0;
  Asm cli; End;
  SDLST:=@DLIST;
  PFCOL0:=$EA; PFCOL1:=$00; PFCOL2:=$0F; PFCOL4:=$e0;
  KRPDEL:=10; KEYREP:=1;// SDMACTL:=%00100010;
//
  initCursor();
  initInterface();
  initModules();
  showAbout();
End;

begin
  initEditor();
  while (true) do
  begin
    if checkZones() then
    begin
      Asm
        lda szone
        asl @
        tay
        lda adr._mzonePROC,y
        sta jaddr
        lda adr._mzonePROC+1,y
        sta jaddr+1
        jsr jaddr:$ffff
      End;
    End;
  End;
End.
