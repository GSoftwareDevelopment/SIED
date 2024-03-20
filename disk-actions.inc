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
var
  zone:shortint absolute $3e;

begin
  if KEYB=255 then
    zone:=szone
  else
    zone:=ozone;
  case zone of
    LISTZONE..LISTZONE+MAXLISTITEMS-1:
    begin
      fn:=dirName[zone-LISTZONE]; updateFileNameField();
    End;
  End;
End;

procedure doEraseFileName(); Keep;
begin
  fn:=''; updateFileNameField();
  czone:=14; // must be set as call zone
  szone:=14; // must be set as select zone
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
  modKey:Byte;
  czone:shortint;

begin
  if _modKey<>0 then exit;
  KEYB:=KEYB and $3F;
  czone:=ozone;
  if (czone<MIN) or (czone>MAX) then
    setCursorInZone(MIN)
  else
  begin
    case KEYB of
      k_UP: if czone>MIN then
              dec(czone)
            else // page change
            if _mZoneActive[LISTZONE-1] then
            begin
              doPrevPageDir();
              czone:=MAX; ozone:=-1;
            end;
      k_DOWN: if czone<MAX then
                inc(czone)
              else
              if _mZoneActive[LISTZONE+MAXLISTITEMS] then
              begin
                doNextPageDir();
                czone:=MIN; ozone:=-1;
              end;
      k_LEFT: if czone-5>=MIN then
                dec(czone,STEP)
              else
              if _mZoneActive[LISTZONE-1] then
              begin
                doPrevPageDir(); ozone:=-1;
              end;
      k_RIGHT: if czone+5<=MAX then
                inc(czone,STEP)
              else
              if _mZoneActive[LISTZONE+MAXLISTITEMS] then
              begin
                doNextPageDir(); ozone:=-1;
              end;
    end;
    setCursorInZone(czone);
  end;
end;