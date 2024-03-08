procedure doSearchPath();
begin
  doInput(pathNameSearch);
  updateSearchField();
  refreshList();
end;

procedure doSelectPath();
var
  name:PString;

begin
  name:=pathNamePtr[listShift+szone-LISTZONE];
  pathNameSearch:=name; updateSearchField();
end;

procedure doEraseSearch();
begin
  pathNameSearch:=''; updateSearchField();
  refreshList();
end;

procedure doPageChange();
begin
  if szone-LISTZONE>=0 then
  begin
    if listShift<(MAXPATHDEFINITIONS-10) then inc(listShift,5) else listShift:=MAXPATHDEFINITIONS-5;
  end
  else
  begin
    if listShift>5 then dec(listShift,5) else listShift:=0;
  end;
  redrawList:=false; showPathList(listShift);
  if foundItems=0 then listShift:=0;
  showPathList(listShift);
end;

procedure doCreate();
var
  p:PByte;
  ln:Byte;

begin
// check for exist name in list
  redrawList:=false; showPathList(0);
  if foundItems<>0 then
  begin
    setStatus(MSG_DUPLICATEDNAME);
    exit;
  end;
// find first free
  i:=0;
  while (pathListPtr[i]<>pointer(0)) and (i<MAXPATHDEFINITIONS) do inc(i);
  if (i=MAXPATHDEFINITIONS) then exit;
  pathListPtr[i]:=pointer($a000);
  p:=pointer(PATHNAMES_ADDR); n:=0;
  while (n<>i) do
  begin
    if p^=0 then begin n:=-1; break; end;
    inc(n);
    inc(p,p^+1);
  end;
  if n=i then
  begin
    pathNamePtr[i]:=p;
    ln:=byte(pathNameSearch[0])+1;
    // move(p,pointer(word(p)+ln),$800-ln);
    move(@pathNameSearch,p,ln);
    listShift:=0; showPathList(0);
  end
  else
  begin
    setStatus(MSG_FULLLIST);
  end;
end;

procedure doEdit();
begin
end;

procedure doName();
begin
end;

procedure doSearch();
begin
end;

procedure doDelete();
begin
end;

procedure doSetAction();
begin
  if curAction<>-1 then invert(_ACTIONX[curAction],_ACTIONY[curAction],_ACTIONH[curAction]-1,11);
  curAction:=szone-9;
  invert(_ACTIONX[curAction],_ACTIONY[curAction],_ACTIONH[curAction]-1,11);
  if pathNameSearch[0]=#0 then
  begin
    setStatus(HELP_BEFOREACTION);
  end
  else
  begin
    case szone of
      9 : doCreate();
      10: doEdit();
      11: doName();
      12: doSearch();
      13: doDelete();
    end;
  end;
end;

procedure toZone(n:byte);
begin
  szone:=n; setCursorInZone(n);
end;

procedure keyName();
begin
  toZone(14); keyb:=$FF; doSearchPath();
end;

procedure keyCreate();
begin
  toZone(9); doSetAction();
end;

procedure keyEdit();
begin
  toZone(10); doSetAction();
end;

procedure keyRename();
begin
  toZone(11); doSetAction();
end;

procedure keySearch();
begin
  toZone(12); doSetAction();
end;

procedure keyDelete();
begin
  toZone(13); doSetAction();
end;
