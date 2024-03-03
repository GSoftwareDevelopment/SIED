const
  _LISTY:Array[0..4] of shortint = (13,20,27,34,41);

var
  listShift:shortint;

procedure updateSearch();
begin
  blank(9,1,PATHNAMESIZE,5);
  if pathNameSearch[0]=#0 then
    putText(9,1,MSG_SEARCH)
  else
    putText(9,1,pathNameSearch);
end;

procedure addPathLabel(n:Byte; name:PString);
var
  y:Byte;

begin
  if n>4 then exit;
  y:=_LISTY[n];
  putText(10,y,name);
  addZoneN(LISTZONE+n,9,y-1,14,7,@doSelectPath);
End;

procedure showPathList();
var
  i,n:byte;
  entry:pointer;
  name:PString;

begin
  n:=listShift; i:=0;
  repeat
    entry:=pathListPtr[n];
    if entry<>pointer(0) then
    begin
      name:=pathNamePtr[n];
      addPathLabel(i,name);
      inc(n); if (n=MAXPATHDEFINITIONS) then break;
    end
    else
    begin
      clearZone(LISTZONE+i);
      blank(9,_LISTY[i],PATHNAMESIZE,5);
    end;
    inc(i);
  until (i=5);
  if n=listShift then putText(14,27,MSG_NOTFOUND);
end;
