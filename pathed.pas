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
  MAXPATHDEFINITIONS  = 64;
  PATHNAMESIZE  = 28;
  LISTZONE = 20;
  MSG_SEARCH ='ENTER PATH NAME HERE...';
  MSG_NOTFOUND = 'NO ENTRIES FOUND :(';

var
  pathListPtr:Array[0..MAXPATHDEFINITIONS] of pointer absolute PATHLISTV_ADDR;
  pathNamePtr:Array[0..MAXPATHDEFINITIONS] of pointer absolute PATHNAMEV_ADDR;
  pathNames:Array[0..0] of char absolute PATHNAMES_ADDR;
  pathNameSearch:String[PATHNAMESIZE];

procedure updateSearch(); forward;
{$I 'pathed-actions.inc'}
{$I 'pathed-controls.inc'}

procedure showPathChoice();
begin
  setStatus('CHOICE PATH TO EDIT OR CREATE NEW...');
  setScreenWidth(20);
  fillchar(YSCR[56+9],20,$FF);
  pathNameSearch:='';
  addInput(9,1,PATHNAMESIZE,pathNameSearch,@doSearchPath);
  updateSearch();
  putText(1,1,'SEARCH:');
  addButton(1,13,'CREATE',@doCreatePath);
  addButton(1,20,'EDIT',@nullProc);
  addButton(1,27,'NAME',@nullProc);
  addButton(1,41,'DELETE',@nullProc);
  putImage(_VSCROLL,19,11,1,36);
  addZone(39,11,1,4,@nullProc);
  addZone(39,43,1,4,@nullProc);
  showPathList();
end;

procedure showPathEditor();
begin
  showPathChoice();
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