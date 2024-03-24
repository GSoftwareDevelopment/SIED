procedure startDirectory;
begin
  dirPageBegin:=0;
  readDirectory();
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
  readDirectory();
End;

procedure doNextPageDir();
begin
  inc(dirPageBegin,MAXLISTITEMS);
  readDirectory();
End;

procedure doSaveToDisk();
var
  src:Pointer;
  blockSize:Word;

begin
  setCursor(_WAIT);
  setStatus(MSG_SAVE);
  fillchar(@_fn,21,$9b);
  _fn:=dev; move(fn[1],_fn[Byte(_fn[0])+1],Byte(fn[0]));

  opn(1,8,0,_fn);
  if IOResult=1 then
  begin
    src:=pointer(CARD_ADDR);
    repeat
      blockSize:=RLECompress(src,@rlebuf,RLEBUFFERSIZE);
      BPut(1,@blockSize,2);
      BPut(1,@rlebuf,blockSize);
      inc(src,RLEBUFFERSIZE);
    until word(src)=$D000;
  end;
  if IOResult<>1 then
  begin
    showIOError();
    cls(1);
  end
  else
  begin
    cls(1);
    readDirectory();
  end;
end;

procedure doLoadFromDisk();
var
  dst:Pointer;
  blockSize:Word;
  moduleInitialized:Byte absolute $62;

begin
  setCursor(_WAIT);
  setStatus(MSG_LOAD);
  fillchar(@_fn,21,$9b);
  _fn:=dev; move(fn[1],_fn[Byte(_fn[0])+1],Byte(fn[0]));

  opn(1,4,0,_fn);
  if IOResult=1 then
  begin
    dst:=pointer(CARD_ADDR);
    repeat
      BGet(1,@blockSize,2);
      BGet(1,@rlebuf,blockSize);
      blockSize:=RLEDecompress(@rlebuf,dst,blockSize);
      inc(dst,blockSize);
    until word(dst)=$d000;
  end;
  if IOResult<>3 then
    showIOError()
  else
  begin
    setStatus(MSG_LOAD_SUCCESS);
    moduleInitialized:=moduleInitialized or $2; // prevent trail module initialize
  end;
  cls(1);
  setCursor(_ARROW);
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
  setCursorInZone(10); doLoadFromDisk();
end;

procedure keySave(); Keep;
begin
  if _modKey<>KMOD_CTRL then exit;
  setCursorInZone(9); doSaveToDisk();
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
    case (KEYB and $3F) of
      k_UP: if nowZone>MIN then
              dec(nowZone)
            else // page change
            if _mZoneActive[LISTZONE-1] then
            begin
              doPrevPageDir();
              nowZone:=MAX; oldZone:=-1;
            end;
      k_DOWN: if nowZone<MAX then
                inc(nowZone)
              else
              if _mZoneActive[LISTZONE+MAXLISTITEMS] then
              begin
                doNextPageDir();
                nowZone:=MIN; oldZone:=-1;
              end;
      k_LEFT: if nowZone-5>=MIN then
                dec(nowZone,STEP)
              else
              if _mZoneActive[LISTZONE-1] then
              begin
                i:=nowZone+5;
                doPrevPageDir(); oldZone:=-1;
                nowZone:=i;
              end;
      k_RIGHT: if nowZone+5<=MAX then
                inc(nowZone,STEP)
              else
              if _mZoneActive[LISTZONE+MAXLISTITEMS] then
              begin
                i:=nowZone-5;
                doNextPageDir(); oldZone:=-1;
                nowZone:=i;
              end;
    end;
    setCursorInZone(nowZone);
  end;
end;