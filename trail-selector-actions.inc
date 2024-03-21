procedure doSearchPath();
begin
  // if doInput(pathNameSearch) and 64=0 then
  // begin
  doInput(pathNameSearch);
    updateSearchField();
    refreshList();
  // end;
end;

procedure doSelectPath();
var
  name:PString;

begin
  n:=curZone-LISTZONE;
  curTrailID:=listNEntry[n];
  name:=pathNamePtr[curTrailID];
  pathNameSearch:=name; updateSearchField();
end;

procedure doEraseSearch();
begin
  pathNameSearch:=''; updateSearchField();
  refreshList();
  // setCursorInZone(13);
  curZone:=13;  // must be set as call zone
  selZone:=13;  // must be set as select zone
  doSearchPath();
end;

procedure doPageChange();
begin
  if selZone-LISTZONE>=0 then
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
      ln:=byte(pathNameSearch[0])+1;
      move(@pathNameSearch,p,ln);
      pathListPtr[i]:=adr; p:=adr;
      poke(adr,$F0); inc(adr);
      poke(adr,$FF); inc(adr);
      poke(adr,$FF); inc(adr);
      poke(adr,$FF); inc(adr);
      poke(adr,$F0); inc(adr);
      poke(adr,$F1); inc(adr);
      poke(adr,$F1); inc(adr);
      poke(adr,$F1); inc(adr);
      poke(adr,$F1); inc(adr);
      poke(adr,$F1); inc(adr);
      poke(adr,$F1); inc(adr);
      poke(adr,$F0); inc(adr);
      poke(adr,$FF); inc(adr);
      poke(adr,$FF); inc(adr);
      poke(adr,$FF); inc(adr);
      poke(adr,$80); inc(adr);
      dpoke(adr,word(p)+1); inc(adr,2);
      pathListPtr[i+1]:=adr;
      // move(p,pointer(word(p)+ln),$800-ln);
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
    setStatus(@MSG_EDIT_NOTRAIL)
  else
  begin
    unregisterShortcutKeys(@selectorShortKeys,9);
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
    case selZone of
      9 : doCreate();
      10: doEdit();
      11: doName();
      12: doDelete();
    end;
  end
  else
  begin
    setStatus(MSG_CERD_NOTRAIL);
  end;
end;

procedure keyName(); Keep;
begin
  if (_modKey<>(KMOD_CTRL)) then exit;
  if setCursorInZone(13) then
  begin
    keyb:=$FF; doSearchPath();
  end;
end;

procedure keyCreate(); Keep;
begin
  if (_modKey<>(KMOD_CTRL)) then exit;
  if setCursorInZone(9) then doSetAction();
end;

procedure keyEdit(); Keep;
begin
  if (_modKey<>(KMOD_CTRL)) then exit;
  if setCursorInZone(10) then doSetAction();
end;

procedure keyRename(); Keep;
begin
  if (_modKey<>(KMOD_CTRL)) then exit;
  if setCursorInZone(11) then doSetAction();
end;

procedure keyDelete(); Keep;
begin
  if (_modKey<>(KMOD_CTRL+KMOD_SHIFT)) then exit;
  if setCursorInZone(12) then doSetAction();
end;

procedure keySelectPath(); Keep;
const
  MIN=LISTZONE;
  MAX=LISTZONE+4;
  STEP=5;

var
  modKey:Byte;
  // curZone:shortint;

begin
  if _modKey<>0 then exit;
  KEYB:=KEYB and $3F;
  // curZone:=oldZone;
  if (curZone<MIN) or (curZone>MAX) then
    setCursorInZone(MIN)
  else
  begin
    case KEYB of
      k_UP: if curZone>MIN then
              dec(curZone)
            else // page change
            if _mZoneActive[LISTZONE-1] then
            begin
              selZone:=LISTZONE-1; doPageChange();
              curZone:=MAX; oldZone:=-1;
            end;
      k_DOWN: if curZone<MAX then
                inc(curZone)
              else
              if _mZoneActive[LISTZONE+5] then
              begin
                selZone:=LISTZONE+5; doPageChange();
                curZone:=MIN; oldZone:=-1;
              end;
    end;
    setCursorInZone(curZone);
  end;
end;