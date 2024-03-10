const
  LISTZONE = 20;
  MSG_SEARCH ='ENTER TRAIL NAME HERE...';
  MSG_NOTFOUND = 'NO ENTRIES FOUND';
  MSG_NAMECREATED = 'NEW ENTRY IS CREATED';
  MSG_DUPLICATEDNAME = 'DUPLICATED TRAIL NAME';
  MSG_FULLLIST = 'LIST IS FULL!';
  MSG_CHOICETRAIL = 'FIRST SELECT TRAIL';
  HELP_TRAILEDITOR = 'TRAIL EDITOR';
  // HELP_BEFOREACTION = 'ENTER TRAIL NAME AND/OR SELECT FROM LIST';
  HELP_ACTION : Array of String = ['CREATE','EDIT','RENAME','SEARCH','DELETE'];
  _ACTIONY: Array[0..3] of Shortint = ( 0,12,24,36 );
  _LISTY:Array[0..4] of shortint = (13,20,27,34,41);

var
  redrawList:Boolean;
  listShift:shortint;
  foundItems:shortint;
  i,n:shortint;
  curAction:shortint;

//
function getTrailName(n:Byte):pointer;
var
  p:PByte;

begin
  // find 'i' entry in table of trail names
  p:=pointer(PATHNAMES_ADDR);
  while (n<>0) do
  begin
    if p^=0 then exit(pointer(0));
    dec(n);
    inc(p,p^+1);
  end;
  result:=p;
end;

procedure updateSearchField(); forward;
procedure refreshList(); forward;
procedure showPathList(n:byte); forward;
procedure selectorShortKeys(); assembler; forward;
{$I 'trail-selector-actions.inc'}
{$I 'trail-selector-controls.inc'}

procedure selectorShortKeys(); assembler;
asm
  dta k_N, a(MAIN.keyName)
  dta k_C, a(MAIN.keyCreate)
  dta k_E, a(MAIN.keyEdit)
  dta k_R, a(MAIN.keyRename)
  dta k_DELETE, a(MAIN.keyDelete)
end;

procedure showTrailSelector();
begin
  registerShortcutKeys(@selectorShortKeys,5);
  setStatus(HELP_TRAILEDITOR);
  setControls(0); // no controls
  setScreenWidth(20);
  fillchar(YSCR[56+8]+4,16,$FF);
  fillchar(Pointer(PMG_ADDR+$300+23),50,$FF);
  HPOSP[2]:=44; PCOL[2]:=$E6; SIZEP[2]:=%11;
  putImage(_ITRLSEL,0,0,3,48);
  putImage(_VSCROLL,19,11,1,3);
  putImage(_VSCROLL+2,19,44,1,3);
  putImage(_ERASEINPUT,19,1,1,5);
  for i:=0 to 3 do
    addZone(0,_ACTIONY[i],6,12,@doSetAction);
  addZoneN(LISTZONE-1,38,10,2,5,@doPageChange);
  addZoneN(LISTZONE+5,38,43,2,5,@doPageChange);
  // szone:=11; doSetAction();
  pathNameSearch:='';
  addInput(9,1,PATHNAMESIZE,pathNameSearch,@doSearchPath);
  addZone(38,0,2,7,@doEraseSearch);
  updateSearchField();
  listShift:=0; showPathList(0);
  if foundItems=0 then
    putText(14,27,MSG_NOTFOUND);
end;
