{$i 'interface.var.inc'}

var
  __x,__y,__w,__h:Byte;

procedure nullProc(); Assembler;
Asm
  lda #true
  sta __invertZone
End;

procedure setZone(n:Shortint; act:Boolean; x,y,w,h:Byte; prc:TZoneProc);
begin
  _mzoneActive[n]:=act;
  _mzoneX1[n]:=x;     __x:=x;
  _mzoneY1[n]:=y;     __y:=y;
  _mzoneX2[n]:=x+w-1; __w:=w;
  _mzoneY2[n]:=y+h-1; __h:=h;
  _mzonePROC[n]:=prc;
  _mzoneInPROC[n]:=@nullProc;
End;

procedure clearZone(n:Byte);
begin
  // setZone(n,false,255,255,255,255,@nullProc);
  _mzoneActive[n]:=false;
  _mzoneX1[n]:=255;
  _mzoneX2[n]:=255;
  _mzoneY1[n]:=255;
  _mzoneY2[n]:=255;
  _mzonePROC[n]:=@nullProc;
  _mzoneInPROC[n]:=@nullProc;
End;

procedure clearWorkZones();
begin
  for i:=USER_ZONES_BEGIN to MAX_ZONES do
    clearZone(i);
End;

procedure initInterface();
begin
  szone:=-1; ozone:=-1; ombutton:=MBUT_NONE;
  for i:=0 to MAX_ZONES do
    clearZone(i);
End;

function getFreeZone:Shortint;
begin
  result:=-1;
  for i:=USER_ZONES_BEGIN to MAX_ZONES do
  begin
    if _mzoneX1[i]=255 then exit(i);
  End;
End;

function addZoneN(n:Shortint; x,y,w,h:Byte; prc:TZoneProc):Shortint;
begin
  if n=-1 then n:=getFreeZone();
  if n<>-1 then setZone(n,true,x,y,w,h,prc);
  result:=n;
End;

function addZone(x,y,w,h:Byte; prc:TZoneProc):Shortint;
begin
  result:=addZoneN(-1,x,y,w,h,prc);
End;

function addZoneH(prc:TZoneProc):Shortint;
begin
  result:=addZone(__x+__w,__y,__w,__h,prc);
End;

function addZoneHN(n:Shortint; prc:TZoneProc):Shortint;
begin
  result:=addZoneN(n,__x+__w,__y,__w,__h,prc);
End;

function addZoneV(prc:TZoneProc):Shortint;
begin
  result:=addZone(__x,__y+__h,__w,__h,prc);
End;

function addZoneVN(n:Shortint; prc:TZoneProc):Shortint;
begin
  result:=addZoneN(n,__x,__y+__h,__w,__h,prc);
End;

procedure callInZone(n:Byte); assembler;
asm
  asl @
  tay
  lda adr._mzoneInPROC,y
  sta jaddr
  lda adr._mzoneInPROC+1,y
  sta jaddr+1
  jmp jaddr:$ffff
end;

procedure refreshZone();
begin
  if (ozone>=USER_ZONES_BEGIN) then
  begin
    if _mZoneActive[ozone] then
    begin
      callInZone(ozone);
      if __invertZone then invertZone(ozone);
    end;
    ozone:=-1;
  End;
  if (nzone>=USER_ZONES_BEGIN) then
  begin
    if _mZoneActive[nzone] then
    begin
      callInZone(nzone);
      if __invertZone then invertZone(nzone);
    end;
    ozone:=nzone;
  End;
End;

procedure setCursorInZone(n:Shortint);
begin
  nzone:=-1;
  szone:=n; refreshZone();
  if not _mZoneActive[n] then exit;
  i:=_mZoneX2[n] shl 2; inc(i,2);
  setCursorPos(i,_mZoneY2[n]-1);
end;

function checkZones:Boolean;

  function pressed:Boolean;
  begin
    szone:=nzone;
    nzone:=-1;
    ombutton:=MBUT_NONE;
    result:=true;
  End;

begin
  result:=false;
  nzone:=-1; szone:=-1;
  for i:=0 to MAX_ZONES do
  begin
    if _mzoneActive[i] then
      if (mx>=_mzoneX1[i]) and (mx<=_mzoneX2[i]) and
          (my>=_mzoneY1[i]) and (my<=_mzoneY2[i]) then
      begin
        nzone:=i; break;
      End;
  End;

  if nzone<>-1 then // take action only when the cursor is in the zone
  begin
    if nzone<USER_ZONES_BEGIN then
    begin // global zone
      refreshZone();
      if (mbutton=MBUT_NONE) then
      begin // trigger free
        if (ombutton=MBUT_LEFT) and (nzone=ozone) then
          result:=pressed(); // action after release trigger
      End
      else
      begin // trigger pressed
        if (nzone<>ozone) then
        begin // action when trigger is pressed
          if nzone<3 then  // for module
          begin
            setIcon(-1);
            putImage(_ICONS[3+nzone],_mzoneX1[nzone],50,3,12);
          End
          else setControl(nzone-3); // for controls

          ozone:=nzone;
          ombutton:=MBUT_LEFT;
        End;
      End;
    End // End global zone
    else
    begin // user zones
      if (mbutton=MBUT_NONE) then
      begin
        if (ombutton=MBUT_LEFT) then result:=pressed();
        if nzone<>ozone then refreshZone();
      End
      else
      begin
        nzone:=-1;
        refreshZone();
        ombutton:=MBUT_LEFT;
      End;
    End;
  End
  else
  begin
    if ozone<>-1 then callInZone(ozone);
    refreshZone();
    if (ombutton=MBUT_LEFT) then
    begin
      setIcon(-1); setControl(-1);
      ozone:=-1;
      ombutton:=MBUT_NONE;
    End;
  End;
End;

procedure callZoneProc(); Assembler;
Asm
  lda szone
  asl @
  tay
  lda adr._mzonePROC,y
  sta jaddr
  lda adr._mzonePROC+1,y
  sta jaddr+1
  jmp jaddr:$ffff
End;

procedure runInterface();
begin
  while (true) do
  begin
    if checkShortcutKeyboard(0) then callShortcutKeyboard();
    if checkZones() then callZoneProc();
  End;
end;
