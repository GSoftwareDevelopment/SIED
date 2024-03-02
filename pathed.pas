{$define BASICOFF}
{$define ROMOFF}
{$define NOROMFONT}
{$LIBRARYPATH 'core/bin/'}

library pathed:$4000;

const
{$I 'data/data-mem.inc'}

{$I 'core/keyboard.var.inc'}
{$I 'core/pmg.var.inc'}

{$I 'core/assets.h.inc'}
// {$I 'core/cursor.h.inc'}
{$I 'core/graph.h.inc'}
{$I 'core/interface.h.inc'}
{$I 'core/controls.h.inc'}

const
  MSG_SEARCH ='ENTER PATH NAME HERE...';

var
  pathListPtr:Array[0..127] of pointer absolute PATHLISTV_ADDR;
  pathNamePtr:Array[0..127] of pointer absolute PATHNAMEV_ADDR;
  pathNames:Array[0..0] of char absolute PATHNAMES_ADDR;
  pathNameSearch:String[30];

procedure doSearchPath();
begin
  doInput(pathNameSearch);
  if pathNameSearch[0]=#0 then
  begin
    blank(1,1,30,5);
    putText(1,1,MSG_SEARCH);
  end;
end;

procedure showPathList();
begin
  setStatus('CHOICE PATH TO EDIT OR CREATE NEW...');
  setScreenWidth(20);
  pathNameSearch:=MSG_SEARCH;
  fillchar(YSCR[56+8],20,$FF);
  addInput(1,1,30,pathNameSearch,@doSearchPath); pathNameSearch:='';
  addButton(33,1,'SEARCH',@nullProc);
  addButton(33,12,'CREATE',@nullProc);
  addButton(35,20,'EDIT',@nullProc);
  addButton(33,28,'CHANGE',@nullProc);
end;

procedure showPathEditor();
begin
  showPathList();
  //  addZoneN(3,1,YCONTROLS,3,7,@nullProc);   // previous
  // addZoneHN(4,@nullProc);                   // play
  // addZoneHN(5,@nullProc);                   // stop
  // addZoneHN(6,@nullProc);                   // next

  //  addZoneN(7,15,YCONTROLS,3,7,@nullProc);  // insert
  // addZoneHN(8,@nullProc);                   // delete

  // setControls(%001111); // only Prev/Play/Stop/Next controls
  // setControl(-1);
  // fillchar(Pointer(PMG_ADDR+$300+23),50,$FF);
  // HPOSP[2]:=44; PCOL[2]:=$E6; SIZEP[2]:=%11;
  // SetScreenWidth(20);
  // putImage(_IPATH,0,0,3,48);

  //  addZone(0,0,3,12,@nullProc); // list
  // addZoneV(@nullProc);          // pen
  // addZoneV(@nullProc);          // path 1
  // addzoneV(@nullProc);          // sprite
  //  addZone(3,0,3,12,@nullProc); // arrow
  // addZoneV(@nullProc);          // line
  // addZoneV(@nullProc);          // path 2
End;

exports
  showPathEditor;

var
  moduleInitialized:Byte absolute $62;

begin
  if (moduleInitialized and $2=0) then
  begin
    fillchar(@pathListPtr,$1000,$00); // clear pathListPtr, pathNamePtr, pathNames
    moduleInitialized:=moduleInitialized or $2;
  end;
end.