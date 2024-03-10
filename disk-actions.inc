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
  szone:=14; doFilename();
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
  setCursorInZone(10);
end;

procedure keySave(); Keep;
begin
  if _modKey<>KMOD_CTRL then exit;
  setCursorInZone(9);
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

procedure keyPrevPageDir(); Keep;
begin
  setCursorInZone(LISTZONE-1); doPrevPageDir();
end;

procedure keyNextPageDir(); Keep;
begin
  setCursorInZone(LISTZONE+MAXLISTITEMS); doNextPageDir();
end;

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