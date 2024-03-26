procedure showIOError();
begin
  _fn:=MSG_IOERR;
  _fn[12]:=_HEX[IOResult shr 4];
  _fn[13]:=_HEX[IOResult and 15];
  setStatus(_fn);
End;

procedure updateFileNameField();
begin
  blank(25,1,12,5);
  putText(25,1,fn);
end;

procedure addFileLabel(n:Byte; prc:TZoneProc);
var
  x,y:Byte;
  name:PString;
  dirSign:byte;

begin
  if n>LISTZONE+MAXLISTITEMS-1 then exit;
  x:=_DIRFILEX[n]; y:=_DIRFILEY[n];
  name:=dirName[n]; dirSign:=byte(name[1]);
  if (dirSign and $80)<>0 then
  begin
    putChar(x-1,y,'<');
    putChar(x+byte(name[0]),y,'>');
  end;
  putText(x,y,name);
  inc(n,LISTZONE);
  addZoneN(n,x-1,y-1,ITEMSIZE,7,prc);
  zoneTabIndex[n]:=0;
End;

procedure updateDirList;
begin
  if getDirectory()>3 then
    showIOError()
  else
  begin
    setScreenWidth(20);
    for j:=0 to MAXLISTITEMS-1 do
    begin
      blank(_DIRFILEX[j]-1,_DIRFILEY[j]-1,ITEMSIZE,7);
      clearZone(LISTZONE+j);
    end;
    j:=0; while (j<=n) do
    begin
      addFileLabel(j,@doChoiceFile);
      inc(j);
    End;
    zoneActive[LISTZONE-1]:=(dirPageBegin>0);
    zoneActive[LISTZONE+MAXLISTITEMS]:=((dirPageBegin+MAXLISTITEMS)<totalFiles);
    if IOResult<3 then clearStatus();
  End;
End;
