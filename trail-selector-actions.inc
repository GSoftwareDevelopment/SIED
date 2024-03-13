procedure doSearchPath();
begin
  if doInput(pathNameSearch) and 64=0 then
  begin
    updateSearchField();
    refreshList();
  end;
end;

procedure doSelectPath();
var
  name:PString;

begin
  n:=sZone-LISTZONE;
  curTrailID:=listNEntry[n];
  name:=pathNamePtr[curTrailID];
  pathNameSearch:=name; updateSearchField();
end;

procedure doEraseSearch();
begin
  pathNameSearch:=''; updateSearchField();
  refreshList(); szone:=13; doSearchPath();
end;

procedure doPageChange();
begin
  if szone-LISTZONE>=0 then
  begin
    if listShift<(MAXPATHDEFINITIONS-10) then
      inc(listShift,5)
     else
      listShift:=MAXPATHDEFINITIONS-5;
  end
  else
  begin
    if listShift>5 then
      dec(listShift,5)
    else
      listShift:=0;
  end;
  redrawList:=false; showPathList(listShift);
  if foundItems=0 then listShift:=0;
  showPathList(listShift);
end;

procedure doCreate();
var
  adr:Pointer;
  p:PByte;
  ln:Byte;

begin
// check for exist name in list
  redrawList:=false; showPathList(0);
  if foundItems=0 then
  begin
    // find first free entry in table of trail definition pointer
    i:=0; adr:=pointer($a000);
    while (i<MAXPATHDEFINITIONS) do
    begin
      if pathListPtr[i]<adr then begin dec(i); break; end;
      adr:=pathListPtr[i];
      inc(i);
    end;
    if (i=MAXPATHDEFINITIONS) then exit;
    p:=getTrailName(i); // find 'i' entry in table of trail names
    if p<>nil then
    begin
      curTrailID:=i;
      pathNamePtr[i]:=p;
      pathListPtr[i]:=adr;
      pathListPtr[i+1]:=pointer($A000);
      ln:=byte(pathNameSearch[0])+1;
      // move(p,pointer(word(p)+ln),$800-ln);
      move(@pathNameSearch,p,ln);
      listShift:=0; showPathList(0);
      p:=@MSG_NAMECREATED;
    end
    else
    begin
      p:=@MSG_FULLLIST;
      curTrailID:=-1;
    end;
  end
  else
    p:=@MSG_DUPLICATEDNAME;
  setStatus(p);
end;

procedure doEdit();
begin
// check for exist name in list
  redrawList:=false; showPathList(0);
  if foundItems=0 then
    setStatus(@MSG_CHOICETRAIL)
  else
  begin
    unregisterShortcutKeys(@selectorShortKeys,6);
    showTrailEditor();
  end;
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
  if byte(pathNameSearch[0])>0 then
  begin
    case szone of
      9 : doCreate();
      10: doEdit();
      11: doName();
      12: doDelete();
    end;
  end;
end;

procedure toZone(n:byte);
begin
  szone:=n; setCursorInZone(n);
end;

procedure keyName(); Keep;
begin
  if (_modKey<>(KMOD_CTRL)) then exit;
  toZone(13); keyb:=$FF; doSearchPath();
end;

procedure keyCreate(); Keep;
begin
  if (_modKey<>(KMOD_CTRL)) then exit;
  toZone(9); doSetAction();
end;

procedure keyEdit(); Keep;
begin
  if (_modKey<>(KMOD_CTRL)) then exit;
  toZone(10); doSetAction();
end;

procedure keyRename(); Keep;
begin
  if (_modKey<>(KMOD_CTRL)) then exit;
  toZone(11); doSetAction();
end;

procedure keyDelete(); Keep;
begin
  if (_modKey<>(KMOD_CTRL+KMOD_SHIFT)) then exit;
  toZone(12); doSetAction();
end;
