const
  YCONTROLS         = 55;

procedure invertZone(i:Shortint);
var
  _w,_h:Byte;

begin
  _w:=_mzoneX2[i]-_mzoneX1[i];
  _h:=_mzoneY2[i]-_mzoneY1[i];
  invert(_mzoneX1[i],_mzoneY1[i],_w,_h);
End;

procedure clearWorkarea();
begin
  fillchar(Pointer(EDITOR_ADDR),48*20,0);
  clearWorkZones();
End;

procedure clearStatus();
begin
  fillchar(YSCR[56+68],40*7,0);
End;

procedure setStatus(s:PString);
begin
  clearStatus();
  setScreenWidth(40);
  putTextC(0,69,s);
End;

procedure setIcon(n:Shortint);
var
  _x:Byte;
  p:Pointer;

begin
  setScreenWidth(40);
  for i:=0 to 2 do
  begin
    _x:=_mzoneX1[i];
    if (_mzoneActive[i]) then
    begin
      if i=n then
      begin
        putImage(@_ICARD,_x-1,48,5,15);
        p:=AICONS[6+n];
      End
      else
        p:=AICONS[i];
      putImage(p,_x,50,3,12);
    End;
  End;
End;

procedure setControls(s:Byte);
begin
  _mzoneActive[3]:=(s and 1<>0);
  _mzoneActive[4]:=(s and 2<>0);
  _mzoneActive[5]:=(s and 4<>0);
  _mzoneActive[6]:=(s and 8<>0);
  _mzoneActive[7]:=(s and 16<>0);
  _mzoneActive[8]:=(s and 32<>0);
End;

procedure setControl(n:Shortint);
var
  _x:Byte;
  p:Pointer;

begin
  setScreenWidth(40);
  for i:=0 to 5 do
  begin
    _x:=_mzoneX1[i+3];
    if i=n then
      p:=ACONTROLS[i+6]
    else
      p:=ACONTROLS[i];
    putImage(p,_x,YCONTROLS,3,8);
  End;
End;

//

function addButton(x,y:Byte; cap:PString; prc:TZoneProc):Shortint;
begin
  putText(x,y,cap);
  result:=addZone(-1,x-1,y-1,Byte(cap[0])+2,7,prc);
End;

//

function addInput(x,y,w:Byte; value:PString; prc:TZoneProc):Shortint;
begin
  putText(x,y,value);
  result:=addZone(-1,x-1,y-1,w+2,7,prc);
End;

function doInput(value:PString):Shortint;
var
  ozone:Shortint;
  cx:Shortint;
  ch,ln,x,y,w:Byte;
  ctm:Byte;
  isCrs:Boolean;

  procedure refreshValue();
  begin
    blank(x,y,w,7);
    putText(x,y+1,value);
    ln:=Byte(value[0]);
  End;

begin
  ozone:=szone; // remember zone id call
  x:=_mzoneX1[szone];
  y:=_mzoneY1[szone];
  w:=(_mzoneX2[szone]-x)-1;
  _mzoneActive[szone]:=false;
  inc(x);
  refreshValue();
  cx:=Byte(ln);
  ctm:=Byte(tm+1); isCrs:=false; mAllowKeys:=false; KEYB:=255;
  repeat
    ch:=x+cx;
    if (tm=ctm) then
    begin
      ctm:=Byte(tm+20);
      invert(ch,y,0,6); isCrs:=not isCrs;
    End;
    checkZones();
    if mbutton<>MBUT_NONE then
    begin
      if isCrs then invert(ch,y,0,6);
      result:=-2; break;
    End;
    if KEYB<>255 then
    begin
      if isCrs then begin invert(ch,y,0,6); isCrs:=false; End;
      case KEYB of
        K_ESC: begin result:=-1; break; End;
        K_RETURN: begin result:=ozone; break; End;
        KMOD_CTRL+K_LEFT: if cx>0 then dec(cx);
        KMOD_CTRL+K_RIGHT: if cx<w then inc(cx);
        K_DELETE:
        begin
          if cx=ln then value[0]:=char(ln-1);
          value[cx]:=#0;
          if cx>0 then dec(cx);
          refreshValue();
        End;
      else
        ch:=keyscan2asc(KEYB);
        if (ch>95) then dec(ch,32);
        if (cx<w) then inc(cx);
        value[cx]:=char(ch);
        if cx>ln then value[0]:=char(cx);
        refreshValue();
      End;
      ctm:=Byte(tm); KEYB:=255;
    End;
  until (false);
  mAllowKeys:=true;
  KEYB:=255;
  _mzoneActive[ozone]:=true;
End;

//
const
  _DIRFILEX:Array of Byte = [  1,  1,  1,  1,  1, 14, 14, 14, 14, 14, 27, 27, 27, 27, 27 ];
  _DIRFILEY:Array of Byte = [ 13, 20, 27, 34, 41, 13, 20, 27, 34, 41, 13, 20, 27, 34, 41 ];

var
  dirName:Array[0..14] of string[12];
  // dirType:Array[0..14] of Byte;

procedure addFileLabel(n:Byte; prc:TZoneProc);
var
  x,y:Byte;

begin
  if n>14 then exit;
  x:=_DIRFILEX[n]; y:=_DIRFILEY[n];
  putText(x,y,dirName[n]);
  addZone(14+n,x-1,y-1,14,7,prc);
End;
