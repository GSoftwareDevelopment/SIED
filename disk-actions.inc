procedure startDirectory;
begin
  dirPageBegin:=0;
  updateDirList();
End;

procedure doDevice();
begin
  if doInput(dev)>=0 then startDirectory();
End;

procedure doFilename();
begin
  doInput(fn);
End;

procedure doChoiceFile(); Keep;
begin
  case curZone of
    LISTZONE..LISTZONE+MAXLISTITEMS-1:
    begin
      fn:=dirName[curZone-LISTZONE]; updateFileNameField();
    End;
  End;
  if selZone<>curZone then curZone:=-1;
End;

procedure doEraseFileName(); Keep;
begin
  fn:=''; updateFileNameField();
  curZone:=14; // must be set as call zone
  selZone:=14; // must be set as select zone
  doFilename();
end;

procedure doPrevPageDir();
begin
  dec(dirPageBegin,MAXLISTITEMS);
  updateDirList();
End;

procedure doNextPageDir();
begin
  inc(dirPageBegin,MAXLISTITEMS);
  updateDirList();
End;

procedure doSaveToDisk();
begin
  setStatus(MSG_SAVE);
  if saveToDisk(fn)<>1 then
    showIOError()
  else
    updateDirList();
end;

procedure doLoadFromDisk();
var
  moduleInitialized:Byte absolute $62;

begin
  setStatus(MSG_LOAD);
  if loadFromDisk(fn)<>3 then
    showIOError()
  else
  begin
    setStatus(MSG_LOAD_SUCCESS);
    moduleInitialized:=moduleInitialized or $2; // prevent trail module initialize
  end;
end;
//

procedure keyDevice(); Keep;
begin
  if _modKey<>KMOD_CTRL then exit;
  setCursorInZone(13); doDevice();
end;

procedure keyFileName(); Keep;
begin
  if _modKey<>KMOD_CTRL then exit;
  setCursorInZone(14); doFilename();
end;

procedure keyLoad(); Keep;
begin
  if _modKey<>KMOD_CTRL then exit;
  // setCursorInZone(10);
  doLoadFromDisk();
end;

procedure keySave(); Keep;
begin
  if _modKey<>KMOD_CTRL then exit;
  // setCursorInZone(9);
  doSaveToDisk();
end;

procedure keyExport(); Keep;
begin
  if _modKey<>KMOD_CTRL then exit;
  setCursorInZone(11);
end;

procedure keyImport(); Keep;
begin
  if _modKey<>KMOD_CTRL then exit;
  setCursorInZone(12);
end;

// procedure keyPrevPageDir(); Keep;
// begin
//   setCursorInZone(LISTZONE-1); doPrevPageDir();
// end;

// procedure keyNextPageDir(); Keep;
// begin
//   setCursorInZone(LISTZONE+MAXLISTITEMS); doNextPageDir();
// end;

procedure keySelectFile(); Keep;
const
  MIN=LISTZONE;
  MAX=LISTZONE+MAXLISTITEMS-1;
  STEP=5;
var
  i:shortint;

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
              doPrevPageDir();
              nowZone:=MAX; oldZone:=-1;
            end;
      k_DOWN: if nowZone<MAX then
                inc(nowZone)
              else
              if zoneActive[LISTZONE+MAXLISTITEMS] then
              begin
                doNextPageDir();
                nowZone:=MIN; oldZone:=-1;
              end;
      k_LEFT: if nowZone-5>=MIN then
                dec(nowZone,STEP)
              else
              if zoneActive[LISTZONE-1] then
              begin
                i:=nowZone+5;
                doPrevPageDir();
                nowZone:=i; oldZone:=-1;
              end;
      k_RIGHT: if nowZone+5<=MAX then
                inc(nowZone,STEP)
              else
              if zoneActive[LISTZONE+MAXLISTITEMS] then
              begin
                i:=nowZone-5;
                doNextPageDir(); oldZone:=-1;
                nowZone:=i;
              end;
    end;
    for i:=0 to MAX_ZONES do if zoneTabIndex[i]=0 then zoneTabIndex[i]:=-128;
    zoneTabIndex[nowZone]:=0; tabZone:=nowZone;
    setCursorInZone(nowZone);
  end;
end;