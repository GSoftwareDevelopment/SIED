const
  MAX_ZONES         = 31;
  USER_ZONES_BEGIN  = 9;

type
  TZoneProc = procedure();

var
  _mzoneActive:Array[0..MAX_ZONES] of Boolean absolute $500;
  _mzoneX1:array[0..MAX_ZONES] of Byte absolute $520;
  _mzoneY1:array[0..MAX_ZONES] of Byte absolute $540;
  _mzoneX2:array[0..MAX_ZONES] of Byte absolute $560;
  _mzoneY2:array[0..MAX_ZONES] of Byte absolute $580;
  _mzonePROC:Array[0..MAX_ZONES] of TZoneProc absolute $5a0;

var
  szone:Shortint absolute $50;
  ozone:Shortint absolute $51;
  __x,__y,__w,__h:Byte;

procedure nullProc(); Begin End;

procedure setZone(n:Shortint; act:Boolean; x,y,w,h:Byte; prc:TZoneProc);
begin
  _mzoneActive[n]:=act;
  _mzoneX1[n]:=x;     __x:=x;
  _mzoneY1[n]:=y;     __y:=y;
  _mzoneX2[n]:=x+w-1; __w:=w;
  _mzoneY2[n]:=y+h-1; __h:=h;
  _mzonePROC[n]:=prc;
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

function addZone(n:Shortint; x,y,w,h:Byte; prc:TZoneProc):Shortint; overload;
begin
  if n=-1 then n:=getFreeZone();
  if n<>-1 then setZone(n,true,x,y,w,h,prc);
  result:=n;
End;

function addZone(x,y,w,h:Byte; prc:TZoneProc):Shortint; overload;
begin
  result:=addZone(-1,x,y,w,h,prc);
End;

function addZoneH(prc:TZoneProc):Shortint; overload;
begin
  result:=addZone(__x+__w,__y,__w,__h,prc);
End;

function addZoneH(n:Shortint; prc:TZoneProc):Shortint; overload;
begin
  result:=addZone(n,__x+__w,__y,__w,__h,prc);
End;

function addZoneV(prc:TZoneProc):Shortint; overload;
begin
  result:=addZone(__x,__y+__h,__w,__h,prc);
End;

function addZoneV(n:Shortint; prc:TZoneProc):Shortint; overload;
begin
  result:=addZone(n,__x,__y+__h,__w,__h,prc);
End;

function checkZones:Boolean;
var
  nzone:Shortint absolute $57;

  procedure refreshZone();
  begin
    if (ozone>=USER_ZONES_BEGIN) then
    begin
      invertZone(ozone);
      ozone:=-1;
    End;
    if (nzone>=USER_ZONES_BEGIN) then
    begin
      invertZone(nzone);
      ozone:=nzone;
    End;
  End;

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
            putImage(AICONS[3+nzone],_mzoneX1[nzone],50,3,12);
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
    refreshZone();
    if (ombutton=MBUT_LEFT) then
    begin
      setIcon(-1); setControl(-1);
      ozone:=-1;
      ombutton:=MBUT_NONE;
    End;
  End;
End;
