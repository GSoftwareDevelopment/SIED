{$I 'controls.var.inc'}

procedure invertZone(i:Shortint);
var
  _w,_h:Byte;

begin
  _w:=_mzoneX2[i]-_mzoneX1[i];
  _h:=_mzoneY2[i]-_mzoneY1[i];
  invert(_mzoneX1[i],_mzoneY1[i],_w,_h);
End;

procedure blankZone(i:Shortint);
var
  _w,_h:Byte;

begin
  _w:=_mzoneX2[i]-_mzoneX1[i];
  _h:=_mzoneY2[i]-_mzoneY1[i];
  blank(_mzoneX1[i],_mzoneY1[i],_w,_h);
end;

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
        putImage(_ICARD,_x-1,48,5,15);
        p:=_ICONS[6+n];
      End
      else
        p:=_ICONS[i];
      putImage(p,_x,50,3,12);
    End;
  End;
End;

procedure setControls(s:Byte);
var
  b:Byte;

begin
  b:=1;
  for i:=3 to 8 do
  begin
    _mzoneActive[i]:=(s and b)<>0;
    _mZoneProc[i]:=@nullProc;
    b:=b shl 1;
  end;
  // _mzoneActive[3]:=(s and 1<>0);  // prev
  // _mzoneActive[4]:=(s and 2<>0);  // play
  // _mzoneActive[5]:=(s and 4<>0);  // stop
  // _mzoneActive[6]:=(s and 8<>0);  // next
  // _mzoneActive[7]:=(s and 16<>0); // insert
  // _mzoneActive[8]:=(s and 32<>0); // delete
End;

procedure setControl(n:Shortint);
var
  _x:Byte;
  p:Pointer;

begin
  setScreenWidth(40);
  for i:=0 to 5 do
  begin
    if _mzoneActive[i+3] then
    begin
      _x:=_mzoneX1[i+3];
      if i=n then
        p:=_CONTROLS[i+6]
      else
        p:=_CONTROLS[i];
      putImage(p,_x,YCONTROLS,3,8);
    end;
  End;
End;

//

function addButton(x,y:Byte; cap:PString; prc:TZoneProc):Shortint;
begin
  putText(x,y,cap);
  result:=addZoneN(-1,x-1,y-1,Byte(cap[0])+2,7,prc);
End;

//
procedure inInputZone();
begin
  __invertZone:=true;
  if (oZone=sZone) then // cursor enter zone
    setCursor(_BEAM)
  else if (oZone<>sZone) then // cursor leave zone
    setCursor(_ARROW);
end;

procedure inInputEditZone();
begin
  __invertZone:=false;
  if (oZone=sZone) then // cursor enter zone
    setCursor(_BEAM)
  else if (oZone<>sZone) then // cursor leave zone
    setCursor(_ARROW);
end;

function addInput(x,y,w:Byte; value:PString; prc:TZoneProc):Shortint;
begin
  putText(x,y,value);
  result:=addZoneN(-1,x-1,y-1,w+2,7,prc);
  if result<>-1 then
    _mzoneInPROC[result]:=@inInputZone;
End;

function doInput(var sEdit:String):Shortint;
var
  _zone:Shortint;
  cx:Shortint;
  ch,ln:Byte;
  ctm:Byte;
  isCrs:Boolean;
  firstKey:Boolean;
  value:String[40] absolute $400;

  procedure refreshValue();
  begin
    ch:=__x+cx-1;
    ln:=Byte(value[0]);
    blank(ch,__y,0,7);
    putChar(ch,__y+1,value[cx]);
    blank(__x+ln,__y,__w-ln,7);
  End;

  procedure drawCursor(n:Boolean);
  begin
    invert(__x+cx,__y,0,6);
    isCrs:=n;
  end;

  procedure clearValue();
  begin
    cx:=0; value[0]:=#0; ln:=0;
  end;

  procedure storeValue();
  begin
    move(value,sEDIT,ln+1);
  end;

begin
  _zone:=szone; // remember zone id call
  if not _mZoneActive[szone] then exit(-3);
  setScreenWidth(20);
  __x:=_mzoneX1[szone];
  __y:=_mzoneY1[szone];
  __w:=(_mzoneX2[szone]-__x)-1;
  _mzoneInPROC[_zone]:=@inInputEditZone;
  inc(__x);
  ln:=byte(sEdit[0]);
  move(sEDIT,value,ln+1);
  cx:=Byte(ln); ctm:=Byte(tm+1); isCrs:=false; KEYB:=255;
  firstKey:=true;
  repeat
    if (tm=ctm) then
    begin
      ctm:=Byte(tm+20);
      drawCursor(not isCrs);
    End;
    if KEYB<>255 then
    begin
      if isCrs then drawCursor(false);
      case KEYB of
        K_ESC:
        begin
          blankZone(_zone); putText(__x,__y+1,sEdit);
          result:=-128; break;
        End;
        K_RETURN:
        begin
          if value<>sEDIT then
          begin
            storeValue();
            result:=_zone;
          end
          else
            result:=_zone or 64;
          break;
        End;
        KMOD_CTRL+K_LEFT: if cx>0 then dec(cx);
        KMOD_CTRL+K_RIGHT: if cx<ln then inc(cx);
        K_DELETE:
        begin
          if firstKey then
            clearValue()
          else
          begin
            if cx=ln then value[0]:=char(ln-1);
            value[cx]:=#0;
            if cx>0 then dec(cx);
          end;
          refreshValue();
        End;
      else
        if firstKey then clearValue();
        ch:=keyscan2asc(KEYB);
        if (ch>95) then dec(ch,32);
        if (cx<__w) then inc(cx);
        value[cx]:=char(ch);
        if cx>ln then value[0]:=char(cx);
        refreshValue();
      End;
      ctm:=Byte(tm);
      KEYB:=255;
      firstKey:=false;
    End;
    checkZones();
    checkShortcutKeyboard(KMOD_CTRL+KMOD_SHIFT);

    if (nzone<>_zone) and ((mbutton<>MBUT_NONE) or (_callShortcutKeys<>$FF)) then
    begin
      if isCrs then drawCursor(isCrs);
      storeValue(); result:=_zone xor $FF;
      break;
    End;
  until (false);
  if _callShortcutKeys=$FF then KEYB:=255;
  _mzoneInPROC[_zone]:=@inInputZone;
End;
