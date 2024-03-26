procedure updateSearchField();
begin
  setScreenWidth(20);
  blank(8,0,PATHNAMESIZE+2,7);
  if pathNameSearch[0]=#0 then
    putText(9,1,MSG_SEARCH)
  else
    putText(9,1,pathNameSearch);
end;

procedure addPathLabel(n:Byte; isChoice:Boolean; name:PString);
var
  y:Byte;

begin
  if n>4 then exit;
  y:=_LISTY[n];
  blank(8,y-1,PATHNAMESIZE+2,7);
  inc(n,LISTZONE);
  // if name<>nil then // BUG: Not implemented in MP
  asm
    lda name
    ora name+1
    bne make
  end;
    zoneTabIndex[n]:=-128;
    listNEntry[n]:=-1;
    clearZone(n);
    exit;
  asm
    make:
  end;
  begin
    if isChoice then
    begin
      putChar(8,y,'*');
      // putChar(8,y,'<');
    end;
    putText(9,y,name);
    addZoneN(n,8,y-1,PATHNAMESIZE+2,7,@doSelectPath);
    zoneTabIndex[n]:=0;
  end;
End;

procedure updateListScroll();
begin
  zoneActive[LISTZONE-1]:=(listShift>0);
  zoneActive[LISTZONE+5]:=(listShift+5<totalItems);
end;

procedure processPathList(start:byte);
begin
  foundItems:=0; n:=0;
  p:=pointer(PATHNAMES_ADDR);
  while (n<MAXPATHDEFINITIONS) do
  begin
    if p^>0 then
    begin
      if n>=start then
      begin
        name:=PString(p);
        if (pathNameSearch[0]=#0) or (findText(pathNameSearch,name)>0) then
        begin
          if (foundItems<5) then
          begin
            listNEntry[foundItems]:=n;
            // listNamePtr[foundItems]:=name; // BUG: MAD Pascal issue; below solution
            asm
              lda foundItems
              asl @
              tay
              lda name
              sta adr.listNamePtr,y
              lda name+1
              sta adr.listNamePtr+1,y
            end;
          end;
          inc(foundItems);
        end;
      end;
      inc(p,p^+1);
    end;
    inc(n);
  end;
  if start=0 then totalItems:=foundItems;
end;

procedure refreshList();
var
  bcd:Byte;

begin
  processPathList(listShift);
  setScreenWidth(20);
  i:=0;
  while (i<5) do
  begin
    if i<foundItems then
    begin
      n:=listNEntry[i];
      name:=listNamePtr[i]; // pathNamePtr[n];
      addPathLabel(i,(n=curTrailID),name);
    end
    else
      addPathLabel(i,false,nil);
    inc(i);
  end;
  updateListScroll();
  if foundItems=0 then
    putText(14,27,MSG_NOTFOUND);
  if listShift=0 then
  begin
    clearStatus();
    bcd:=BIN2BCD(foundItems);
    setStatus(MSG_ITEMSFOUND);
    if (bcd>=$10) then
      putCharC(0,69,char(48+bcd shr 4));
    putCharC(1,69,char(48+bcd and $f));
  end;
end;
