procedure updateSearchField();
begin
  blank(9,1,PATHNAMESIZE,5);
  if pathNameSearch[0]=#0 then
    putText(9,1,MSG_SEARCH)
  else
    putText(9,1,pathNameSearch);
end;

procedure refreshList();
begin
  listShift:=0; showPathList(0);
  if foundItems=0 then
    setStatus(MSG_NOTFOUND)
  else
    clearStatus();
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
    blank(8,_LISTY[i]-1,PATHNAMESIZE+1,6);
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
  name:PString;
  p:PByte;

begin
  if redrawList then blankList();
  foundItems:=0;
  p:=pointer(PATHNAMES_ADDR); i:=0;
  while (n<MAXPATHDEFINITIONS) do
  begin
    if p^>0 then
    begin
      name:=PString(p);
      if (i<5) then
      begin
        if (pathNameSearch[0]=#0) or (findText(pathNameSearch,name)>0) then
        begin
          if redrawList and (foundItems<5) then addPathLabel(foundItems,name);
          inc(foundItems);
          listNEntry[i]:=n; inc(i);
        end;
      end;
      inc(p,p^+1);
    end;
    inc(n);
  end;
  if redrawList then updateListScroll();
  redrawList:=true;
end;