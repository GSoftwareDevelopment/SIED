procedure doSearchPath();
begin
  doInput(pathNameSearch);
  updateSearchField();
  listShift:=0; refreshList();
end;

function doSelectPath():Boolean; Register;
begin
  n:=curZone-LISTZONE;
  n:=listNEntry[n];
  if curTrailID=n then
    curTrailID:=-1
  else
    curTrailID:=n;
  refreshList();
  result:=true; // execute key shortcut
end;

function doEraseSearch():Boolean; Register;
begin
  pathNameSearch:=''; updateSearchField();
  curTrailID:=-1;
  refreshList();
  curZone:=13; selZone:=curZone; // must be set as call zone
  doSearchPath();
  result:=true; // execute key shortcut
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
  processPathList(listShift);
  if foundItems=0 then listShift:=0;
  refreshList();
end;

//
//
//

procedure doCreate();
begin
  if pathNameSearch[0]=#0 then
    p:=@MSG_CREATE_NONANE
  else
  begin
  // check for exist name in list
    redrawList:=false; processPathList(0);
    if foundItems=0 then
    begin
      // find first free entry in table of trail definition pointer
      i:=getFirstFreeTrailID();
      if i<>-1 then
      begin
        name:=getTrailName(i); // find 'i' entry in table of trail names
        // if name<>nil then // BUG: MP not implemented
        asm
          lda name
          ora name+1
          jeq @fullList
        end;
        begin
          curTrailID:=i;
          // copy path name to name area
          move(pathNameSearch,pointer(name),byte(pathNameSearch[0])+1); // BUG: MP PString problem
          // set path definition start address
          pathListPtr[i]:=adr;
          // create path base
          poke(adr,$00); inc(adr);
          poke(adr,TR_STOP); inc(adr);
          pathListPtr[i+1]:=adr;
          listShift:=0; refreshList();
          p:=@MSG_NAMECREATED;
        end;
        // else
          // p:=@MSG_FULLLIST;
      end
      else
      begin
        asm
          @fullList:
        end;
        p:=@MSG_FULLLIST;
      end;
    end
    else
      p:=@MSG_DUPLICATEDNAME;
  end;
  setStatus(p);
end;

procedure doEdit();
begin
  if curTrailID=-1 then
    setStatus(@MSG_EDIT_NOTRAIL)
  else
  begin
    unregisterShortcutKeys(@selectorShortKeys);
    unregisterTabControl();
    KEYB:=$FF;
    showTrailEditor();
  end;
end;

procedure doName();
begin
end;

procedure doDelete();
begin
end;

//
//
//

function keyName():Boolean; Keep; Register;
begin
  if (_modKey<>(KMOD_CTRL)) then exit;
  if setCursorInZone(13) then
  begin
    keyb:=$FF; doSearchPath();
  end;
  result:=true; // execute key shortcut
end;

function keyCreate():Boolean; Keep; Register;
begin
  if (_modKey<>(KMOD_CTRL)) then exit;
  doCreate();
end;

function keyEdit():Boolean; Keep; Register;
begin
  if (_modKey<>(KMOD_CTRL)) then exit;
  doEdit();
end;

function keyRename():Boolean; Keep; Register;
begin
  if (_modKey<>(KMOD_CTRL)) then exit;
  doName();
end;

function keyDelete():Boolean; Keep; Register;
begin
  if (_modKey<>(KMOD_CTRL+KMOD_SHIFT)) then exit;
  doDelete();
end;

function keySelectPath():Boolean; Keep; Register;
const
  MIN=LISTZONE;
  MAX=LISTZONE+4;
  STEP=5;

begin
  if _modKey<>0 then exit;
  if (nowZone<MIN) or (nowZone>MAX) then
    setCursorInZone(MIN)
  else
  begin
    case KEYB of
      k_UP: if nowZone>MIN then
              dec(nowZone)
            else // page change
            if zoneActive[LISTZONE-1] then
            begin
              selZone:=LISTZONE-1; doPageChange();
              nowZone:=MAX; oldZone:=-1;
            end;
      k_DOWN: if nowZone<MAX then
              begin
                if zoneActive[nowZone+1] then inc(nowZone);
              end
              else
              if zoneActive[LISTZONE+5] then
              begin
                selZone:=LISTZONE+5; doPageChange();
                nowZone:=MIN; oldZone:=-1;
              end;
    end;
    for i:=0 to MAX_ZONES do if zoneTabIndex[i]=0 then zoneTabIndex[i]:=-128;
    zoneTabIndex[nowZone]:=0; tabZone:=nowZone;
    setCursorInZone(nowZone);
  end;
  result:=true; // execute key shortcut
end;