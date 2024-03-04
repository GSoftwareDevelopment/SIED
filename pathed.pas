{$define BASICOFF}
{$define ROMOFF}
{$LIBRARYPATH 'core/bin/'}

library pathed:$4000;

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
  MAXPATHDEFINITIONS  = 64;
  PATHNAMESIZE  = 28;
  LISTZONE = 20;
  MSG_SEARCH ='ENTER TRAIL NAME HERE...';
  MSG_NOTFOUND = 'NO ENTRIES FOUND :(';
  MSG_DUPLICATEDNAME = 'DUPLICATED TRAIL NAME';
  MSG_FULLLIST = 'LIST IS FULL!';
  HELP_BEFOREACTION = 'ENTER TRAIL NAME AND/OR SELECT FROM LIST';
  // ACTION : Array of String = ['CREATE','EDIT','RENAME','SEARCH','DELETE'];
  _ACTIONX: Array[0..4] of Shortint = ( 0 ,0 ,0, 3, 0 );
  _ACTIONY: Array[0..4] of Shortint = ( 0,12,24,24,36 );
  _ACTIONH: Array[0..4] of Shortint = ( 6 ,6 ,3, 3, 6 );
  _LISTY:Array[0..4] of shortint = (13,20,27,34,41);


var
  pathListPtr:Array[0..MAXPATHDEFINITIONS] of pointer absolute PATHLISTV_ADDR;
  pathNamePtr:Array[0..MAXPATHDEFINITIONS] of pointer absolute PATHNAMEV_ADDR;
  pathNames:Array[0..0] of char absolute PATHNAMES_ADDR;
  pathNameSearch:String[PATHNAMESIZE];
  redrawList:Boolean;
  listShift:shortint;
  foundItems:shortint;
  i,n:shortint;
  curAction:shortint;

//

procedure updateSearchField(); forward;
procedure showPathList(n:byte); forward;
{$I 'pathed-actions.inc'}
{$I 'pathed-controls.inc'}

procedure showPathChoice();
begin
  addShortcutKey(k_N,@keyName);
  addShortcutKey(k_C,@keyCreate);
  addShortcutKey(k_E,@keyEdit);
  addShortcutKey(k_R,@keyRename);
  addShortcutKey(k_S,@keySearch);
  addShortcutKey(k_DELETE,@keyDelete);
  setStatus(HELP_BEFOREACTION);
  setScreenWidth(20);
  fillchar(YSCR[56+8]+4,15,$FF);
  fillchar(Pointer(PMG_ADDR+$300+23),50,$FF);
  HPOSP[2]:=44; PCOL[2]:=$E6; SIZEP[2]:=%11;
  putImage(_IPATH,0,0,3,48);
  putImage(_VSCROLL,19,11,1,3);
  putImage(_VSCROLL+2,19,44,1,3);
  for i:=0 to 4 do
    addZone(_ACTIONX[i],_ACTIONY[i],_ACTIONH[i],12,@doSetAction);
  addZoneN(LISTZONE-1,38,10,2,5,@doPageChange);
  addZoneN(LISTZONE+5,38,43,2,5,@doPageChange);
  szone:=12; doSetAction();
  pathNameSearch:='';
  addInput(9,1,PATHNAMESIZE,pathNameSearch,@doSearchPath);
  // addButton(38,1,' ',@nullProc);
  updateSearchField();
  listShift:=0; showPathList(0);
  if foundItems=0 then
    putText(14,27,MSG_NOTFOUND);
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
  [volatile] PORTB:Byte absolute $D301;

begin
  if (moduleInitialized and $2=0) then
  begin
    asm
      lda PORTB
      pha
      and #$FE
      sta PORTB
    end;
    fillchar(pointer(PATHLISTV_ADDR),$1000,$00); // clear pathListPtr, pathNamePtr, pathNames
    moduleInitialized:=moduleInitialized or $2;
    asm
      pla
      sta PORTB
    end;
  end;
  redrawList:=true; curAction:=-1;
end.