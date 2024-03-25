{$i 'interface.var.inc'}

var
  __x,__y,__w,__h:Byte;

procedure nullProc(); Assembler; asm end;

//
procedure setStatusBuf(adr:word); inline;
begin
  dpoke(DLIST_ADDR+$51,adr);
end;

procedure clearHintInZone(n:shortint); Assembler;
asm
  lda n
  asl @
  tay
  lda #0 // nil
  sta adr.zoneHint,y
  lda #0 // nil
  sta adr.zoneHint+1,y
end;

procedure assignHintToZone(n:shortint; hint:PString); Assembler;
asm
  lda n
  asl @
  tay
  lda hint
  sta adr.zoneHint,y
  lda hint+1
  sta adr.zoneHint+1,y
end;
//

procedure setZone(n:Shortint; act:Boolean; x,y,w,h:Byte; prc:TZoneProc);
begin
  zoneActive[n]:=act;
  _mZoneX1[n]:=x;     __x:=x;
  _mZoneY1[n]:=y;     __y:=y;
  _mZoneX2[n]:=x+w-1; __w:=w;
  _mZoneY2[n]:=y+h-1; __h:=h;
  zoneTabIndex[n]:=-128;
  zoneCall[n]:=prc;
  zoneInOutCall[n]:=@doHintZone;
  clearHintInZone(n); // zoneHint[n]:=nil;  // BUG: MP issue
  if (n=hintZone) then begin hintZone:=-1; setStatusBuf(STATUS_ADDR); end;
  // if (n=tabZone) then tabZone:=-1;
  if (nowZone=n) and (oldZone=n) then begin nowZone:=-1; oldZone:=-1; end;
End;

procedure clearZone(n:Byte);
begin
  zoneActive[n]:=false;
  _mZoneX1[n]:=255;
  _mZoneX2[n]:=255;
  _mZoneY1[n]:=255;
  _mZoneY2[n]:=255;
  zoneTabIndex[n]:=-128;
  zoneCall[n]:=@nullProc;
  zoneInOutCall[n]:=@doHintZone;
  clearHintInZone(n); // zoneHint[n]:=nil;  // BUG: MP Issue
  if (n=hintZone) then begin hintZone:=-1; setStatusBuf(STATUS_ADDR); end;
  // if (n=tabZone) then tabZone:=-1;
  if (nowZone=n) and (oldZone=n) then begin nowZone:=-1; oldZone:=-1; end;
End;

procedure clearWorkZones();
begin
  for i:=USER_ZONES_BEGIN to MAX_ZONES do
    clearZone(i);
End;

procedure initInterface();
begin
  selZone:=-1; oldZone:=-1; curZone:=-1; ombutton:=MBUT_NONE;
  hintZone:=-1; tabZone:=-1; tabIndex:=-128;

  for i:=0 to MAX_ZONES do
    clearZone(i);
End;

function getFreeZone:Shortint;
begin
  result:=-1;
  for i:=USER_ZONES_BEGIN to MAX_ZONES do
  begin
    if _mZoneX1[i]=255 then exit(i);
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
  lda adr.zoneInOutCall,y
  sta jaddr
  lda adr.zoneInOutCall+1,y
  sta jaddr+1
  jmp jaddr:$ffff
end;

procedure refreshZone();
begin
  if (oldZone<>-1) then
  begin
    if zoneActive[oldZone] then
    begin
      callInZone(oldZone);
      if (oldZone>=USER_ZONES_BEGIN) then
      begin
        if allowInverZone then invertZone(oldZone);
      end;
    end;
    oldZone:=-1;
  End;
  if (nowZone<>-1) then
  begin
    if zoneActive[nowZone] then
    begin
      callInZone(nowZone);
      if (nowZone>=USER_ZONES_BEGIN) then
      begin
        if allowInverZone then invertZone(nowZone);
      end;
    end;
    oldZone:=nowZone;
  End;
End;

function setCursorInZone(n:Shortint):boolean;
begin
  result:=false;
  nowZone:=-1;
  curZone:=n; selZone:=n;
  refreshZone();
  if not zoneActive[n] then exit;
  curZone:=n;
  tabIndex:=zoneTabIndex[n];
  tabZone:=n;
  i:=_mZoneX2[n] shl 2; inc(i,2);
  setCursorPos(i,_mZoneY2[n]-1);
  result:=true;
end;

function checkZones:Boolean;

  function pressed:Boolean;
  begin
    selZone:=nowZone;
    nowZone:=-1;
    ombutton:=MBUT_NONE;
    result:=true;
  End;

begin
  result:=false;
  nowZone:=-1; selZone:=-1;
  for i:=0 to MAX_ZONES do
  begin
    if zoneActive[i] then
      if (mx>=_mZoneX1[i]) and (mx<=_mZoneX2[i]) and
          (my>=_mZoneY1[i]) and (my<=_mZoneY2[i]) then
      begin
        nowZone:=i; break;
      End;
  End;

  if nowZone<>-1 then // take action only when the cursor is in the zone
  begin
    if nowZone<USER_ZONES_BEGIN then
    begin // global zone
      refreshZone();
      if (mbutton=MBUT_NONE) then
      begin // trigger free
        if (ombutton=MBUT_LEFT) and (nowZone=oldZone) then
          result:=pressed(); // action after release trigger
      End
      else
      begin // trigger pressed
        if (omButton=MBUT_NONE) then
        begin // action when trigger is pressed
          if nowZone<3 then  // for module
          begin
            setIcon(-1);
            putImage(_ICONS[3+nowZone],_mZoneX1[nowZone],50,3,12);
          End
          else setControl(nowZone-3); // for controls

          oldZone:=nowZone;
          ombutton:=MBUT_LEFT;
        End;
      End;
    End // End global zone
    else
    begin // user zones
      if (mbutton=MBUT_NONE) then
      begin
        if (ombutton=MBUT_LEFT) then result:=pressed();
        if nowZone<>oldZone then refreshZone();
      End
      else
      begin
        nowZone:=-1;
        refreshZone();
        ombutton:=MBUT_LEFT;
      End;
    End;
  End
  else
  begin
    // if oldZone<>-1 then callInZone(oldZone);
    refreshZone();
    if (ombutton=MBUT_LEFT) then
    begin
      setIcon(-1); setControl(-1);
      oldZone:=-1;
      ombutton:=MBUT_NONE;
    End;
  End;
End;

procedure callZoneProc(); Assembler;
Asm
  lda selZone
  sta curZone
  sta tabZone
  tay
  lda adr.zoneTabIndex,y
  sta tabIndex
  tya
  asl @
  tay

  lda adr.zoneCall,y
  sta jaddr
  lda adr.zoneCall+1,y
  sta jaddr+1

  jsr jaddr:$ffff

  lda curZone
  cmp nowZone
  bne @exit
  cmp oldZone
  beq @+
clroZone:
  lda #(-1)
  sta oldZone
  bmi @exit
@:
  lda nowZone
  cmp oldZone
  beq clroZone
End;

//

procedure runInterface();
begin
  while (true) do
  begin
    checkTimers();
    if checkShortcutKeyboard(0) then callShortcutKeyboard();
    if checkZones() then callZoneProc();
  End;
end;
