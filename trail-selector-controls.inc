procedure updateSearchField();
begin
  setScreenWidth(20);
  blank(8,1,PATHNAMESIZE+1,5);
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
  if isChoice then
  begin
    putChar(8,y,'>');
    putChar(8,y,'<');
  end;
  putText(9,y,name);
  inc(n,LISTZONE);
  addZoneN(n,8,y-1,PATHNAMESIZE+2,7,@doSelectPath);
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
  _mZoneActive[LISTZONE-1]:=(listShift>0);
  _mZoneActive[LISTZONE+5]:=(listShift+5<totalItems);
end;

procedure showPathList(start:byte);
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
  showPathList(listShift);
  setScreenWidth(20);
  blankList();
  i:=0;
  while (i<foundItems) do
  begin
    n:=listNEntry[i];
    name:=listNamePtr[i]; // pathNamePtr[n];
    addPathLabel(i,(n=curTrailID),name);
    inc(i);
  end;
  updateListScroll();
  if foundItems=0 then
    putText(14,27,MSG_NOTFOUND);
  // else
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
