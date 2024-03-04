procedure updateSearchField();
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
  putText(9,y,name);
  addZoneN(LISTZONE+n,8,y-1,PATHNAMESIZE+2,7,@doSelectPath);
End;

procedure blankList();
begin
  for i:=0 to 4 do
  begin
    blank(9,_LISTY[i],PATHNAMESIZE,5);
    clearZone(LISTZONE+i);
  end;
end;

procedure updateListScroll();
begin
  _mzoneActive[LISTZONE-1]:=(listShift>0);
  _mzoneActive[LISTZONE+5]:=(listShift+5<foundItems);
end;

procedure showPathList(n:byte);
var
  entry:pointer;
  name:PString;

begin
  if redrawList then blankList();
  foundItems:=0;
  while (n<MAXPATHDEFINITIONS) and (i<=5) do
  begin
    entry:=pathListPtr[n];
    if entry<>pointer(0) then
    begin
      name:=pathNamePtr[n];
      if (pathNameSearch[0]=#0) or (findText(pathNameSearch,name)>0) then
      begin
        if redrawList and (foundItems<5) then addPathLabel(foundItems,name);
        inc(foundItems);
      end;
    end;
    inc(n);
  end;
  if redrawList then
  begin
    updateListScroll()
  end;
  redrawList:=true;
end;
