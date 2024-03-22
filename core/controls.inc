{$I 'controls.var.inc'}

procedure invertZone(i:Shortint);
var
  _w,_h:Byte;

begin
  _w:=_mZoneX2[i]-_mZoneX1[i];
  _h:=_mZoneY2[i]-_mZoneY1[i];
  invert(_mZoneX1[i],_mZoneY1[i],_w,_h);
End;

procedure blankZone(i:Shortint);
var
  _w,_h:Byte;

begin
  _w:=_mZoneX2[i]-_mZoneX1[i];
  _h:=_mZoneY2[i]-_mZoneY1[i];
  blank(_mZoneX1[i],_mZoneY1[i],_w,_h);
end;

procedure clearWorkarea();
begin
  fillchar(Pointer(EDITOR_ADDR),48*20,0);
  clearWorkZones();
  nowZone:=-1; oldZone:=-1; selZone:=-1;
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

procedure doHintZone(); Keep;
begin
  allowInverZone:=(selZone=nowZone) or (oldZone<>nowZone);
  // if (nowZone=oldZone) then // cursor enter zone
  // begin
  //   if _mZoneHint[nowZone]<>nil then
  //     setStatus(_mZoneHint[nowZone]);
  // end
  // else if (nowZone<>oldZone) then // cursor leave zone
  // begin
  //   clearStatus();
  // end;
end;

procedure setIcon(n:Shortint);
var
  _x:Byte;
  p:Pointer;

begin
  setScreenWidth(40);
  for i:=0 to 2 do
  begin
    _x:=_mZoneX1[i];
    if (_mZoneActive[i]) then
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
    _mZoneActive[i]:=(s and b)<>0;
    _mZoneCall[i]:=@nullProc;
    b:=b shl 1;
  end;
  // _mZoneActive[3]:=(s and 1<>0);  // prev
  // _mZoneActive[4]:=(s and 2<>0);  // play
  // _mZoneActive[5]:=(s and 4<>0);  // stop
  // _mZoneActive[6]:=(s and 8<>0);  // next
  // _mZoneActive[7]:=(s and 16<>0); // insert
  // _mZoneActive[8]:=(s and 32<>0); // delete
End;

procedure setControl(n:Shortint);
var
  _x,_y:Byte;
  p:Pointer;

begin
  setScreenWidth(40);
  for i:=0 to 5 do
  begin
    if _mZoneActive[i+3] then
    begin
      _x:=_mZoneX1[i+3];
      _y:=_mZoneY1[i+3];
      if i=n then
        p:=_CONTROLS[i+6]
      else
        p:=_CONTROLS[i];
      putImage(p,_x,_y,3,8);
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
  allowInverZone:=true;
  if (oldZone=selZone) then // cursor enter zone
    setCursor(_BEAM)
  else if (oldZone<>selZone) then // cursor leave zone
    setCursor(_ARROW);
end;

procedure inInputEditZone();
begin
  allowInverZone:=false;
  if (oldZone=selZone) then // cursor enter zone
    setCursor(_BEAM)
  else if (oldZone<>selZone) then // cursor leave zone
    setCursor(_ARROW);
end;

function addInput(x,y,w:Byte; value:PString; prc:TZoneProc):Shortint;
begin
  putText(x,y,value);
  result:=addZoneN(-1,x-1,y-1,w+2,7,prc);
  if result<>-1 then
    _mZoneInCall[result]:=@inInputZone;
End;

function doInput(var sEdit:String):Shortint;
var
  // curZone:Shortint;
  cx:Shortint;
  ch,ln:Byte;
  ctm:Byte;
  isCrs:Boolean;
  firstKey:Boolean;
  value:String[40] absolute $400;
  _exit:Boolean;

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
  // curZone:=selZone; // remember zone id call
  if not _mZoneActive[selZone] then exit(-3);
  setScreenWidth(20);
  __x:=_mZoneX1[selZone];
  __y:=_mZoneY1[selZone];
  __w:=(_mZoneX2[selZone]-__x)-1;
  _mZoneInCall[curZone]:=@inInputEditZone;
  inc(__x);
  ln:=byte(sEdit[0]);
  move(sEDIT,value,ln+1);
  cx:=Byte(ln); ctm:=Byte(tm+1); isCrs:=false; KEYB:=255;
  firstKey:=true; _exit:=false;
  repeat
    if (tm=ctm) then
    begin
      ctm:=Byte(tm+20);
      drawCursor(not isCrs);
    End;

    // checkShortcutKeyboard(0); // (KMOD_CTRL+KMOD_SHIFT);
    checkZones();
    if (nowZone<>curZone) and (mbutton<>MBUT_NONE) then
    begin
      _exit:=true; break;
    end;

    if (KEYB<>255) then
    begin
      if isCrs then drawCursor(false);
      allowShortcutKeys:=true;
      _modKey:=KEYB and (KMOD_CTRL+KMOD_SHIFT);
      KEYB:=KEYB and $3f;
      if _modKey=KMOD_CTRL then
      begin
        case KEYB of
          K_LEFT:  if cx>0  then begin dec(cx); allowShortcutKeys:=false; end;
          K_RIGHT: if cx<ln then begin inc(cx); allowShortcutKeys:=false; end;
        end;
      end
      else
      begin
        case KEYB of
          K_ESC:
          begin
            blankZone(curZone); putText(__x,__y+1,sEdit);
            result:=-128; break;
          End;
          K_RETURN:
          begin
            if value<>sEDIT then
            begin
              storeValue();
              result:=curZone;
            end
            else
              result:=curZone or 64;
            break;
          End;
          K_DELETE:
          begin
            allowShortcutKeys:=false;
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
          if (_modKey=0) or (_modKey=KMOD_SHIFT) then
          begin
            if _modKey=KMOD_SHIFT then inc(KEYB,KMOD_SHIFT);
            if firstKey then clearValue();
            ch:=keyscan2asc(KEYB);
            if (ch>95) then dec(ch,32);
            if (cx<__w) then inc(cx);
            value[cx]:=char(ch);
            if cx>ln then value[0]:=char(cx);
            refreshValue();
            allowShortcutKeys:=false;
          end;
        End;
      end;
      if checkShortcutKeyboard(KMOD_CTRL+KMOD_SHIFT) then
      begin
        _exit:=true; break;
      end;
      ctm:=Byte(tm);
      KEYB:=255;
      firstKey:=false;
    End;
  until (false);
  if _exit then
  begin
    if isCrs then drawCursor(false);
    storeValue(); result:=curZone xor $FF;
  End;

  if _callShortcutKeys=$FF then KEYB:=255;
  _mZoneInCall[curZone]:=@inInputZone;
End;
