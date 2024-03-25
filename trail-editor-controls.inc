procedure clearSection();
begin
  wait(1);
  for i:=0 to 3 do HPOSM[i]:=0;
  HPOSP[3]:=0;
  for i:=14 to 31 do clearZone(i);
  for i:=56 to 56+47 do fillchar(Pointer(word(YSCR[i])+3),17,0);
end;

procedure clearGameScreen();
begin
  fillchar(Pointer(SCREEN_ADDR),$380,0);
end;

procedure updateGameScreen();
begin
  HPOSM[2]:=48+XVIEWPORT;
  HPOSM[3]:=48+88+8+XVIEWPORT;
  asm
  lline:
    ldy #12
  lcpy:
    :48 mva SCREEN_ADDR+((4+#)*16),y EDITOR_ADDR+(XVIEWPORT/8)+(#*20),y

    dey
    bmi xit
    jmp lcpy
  xit:
  end;
end;

function updatePathSection():boolean;
begin
  if curSection=(selZone-10) then exit(false);
  result:=true;
  curSection:=selZone-10;
  KEYB:=$FF;  // Prevent a previous key click from overriding
  unregisterTabControl();
  zoneActive[11]:=(curSection<>1);
  zoneActive[12]:=(curSection<>2);
  zoneActive[13]:=(curSection<>3);
  case curSection of
    1: scr:=@_TRAYLEFT+24; // Path Begin
    2: scr:=@_TRAYLEFT+12; // Inner Path
    3: scr:=@_TRAYLEFT+ 0; // Path Finish
  else
    // nothing
    fillchar(Pointer(PMG_ADDR+$300+23),50,$FF);
    exit;
  end;
  move(scr,Pointer(PMG_ADDR+$300+24),48);
end;

//
//
//

procedure updateStartPoint();
begin
  n:=pathStartEdge[curTrailID];
  zoneActive[14]:=(n<>0);
  zoneActive[15]:=(n<>1);
  zoneActive[16]:=(n<>2);
  zoneActive[17]:=(n<>3);
  case n of
    EDGE_TOP    : scr:=@_TRAYRIGHT+36;
    EDGE_LEFT   : scr:=@_TRAYRIGHT+24;
    EDGE_RIGHT  : scr:=@_TRAYRIGHT+12;
    EDGE_BOTTOM : scr:=@_TRAYRIGHT+0;
  else
    // nothingclear
    fillchar(Pointer(PMG_ADDR+$380+23),50,$3F);
    exit;
  end;
  move(scr,Pointer(PMG_ADDR+$380+24),48);
end;

//
//
//

procedure updateCoord(x,y,v:Shortint);
begin
  s:='  ';
  if v>=0 then bcd:=bin2bcd(v)
  else
  begin s[1]:='-'; bcd:=bin2bcd(-v); end;
  if bcd>$10 then s[1]:=char(48+bcd shr 4);
  s[2]:=char(48+bcd and $f);
  // blank(x*2,y,3,4);
  putTextC(x,y,s);
end;

procedure updateTimeLineTime();
begin
  frm:=curTLPos mod FPS;
  sec:=curTLPos div FPS;
  min:=sec div 60;
  for i:=56+63 to 56+67 do fillchar(YSCR[i],8,0);
  s:='  :  .  ';
  bcd:=bin2bcd(min);
  s[1]:=char(48+bcd shr 4);
  s[2]:=char(48+bcd and $f);
  bcd:=bin2bcd(sec);
  s[4]:=char(48+bcd shr 4);
  s[5]:=char(48+bcd and $f);
  bcd:=bin2bcd(frm);
  s[7]:=char(48+bcd shr 4);
  s[8]:=char(48+bcd and $f);
  putTextC(0,63,s);
end;

procedure updateStepInfo();
begin
  setScreenWidth(40);
  if (curPX<>_curPX) or (curPY<>_curPY) then
  begin
    putImage(@_ICOORD,11,48,8,6);
    updateCoord(14,48,curPX); updateCoord(17,48,curPY);
    _curPX:=curPX; _curPY:=curPY;
  end;

  if (curDX<>_curDX) or (curDY<>_curDY) then
  begin
    putImage(@_ICOORD+48,19,48,8,6);
    updateCoord(21,48,curDX); updateCoord(24,48,curDY);
    _curDX:=curDX; _curDY:=curDY;
  end;
  setScreenWidth(20);
end;

function processTrail(mode:Byte):Smallint;
var
  w,pathPattern:longint;

begin
  if (mode and PT_UPDATEPATH<>0) then clearGameScreen();
  ntm:=tm+1;
  pathPattern:=trailPattern;
  if (mode and PT_UPDATETIMELINE<>0) then
  begin
    TLPtr:=pointer(TLBUF_ADDR);
    for i:=0 to 4 do
    begin
      fillchar(TLPtr,TIMELINE_SIZEB,_TLLOOP[i]);
      inc(TLPtr,TIMELINE_SIZEB);
    end;
  end;
  // if (mode and PT_KEEPCURSOR=0) then setCursor(_WAIT);
  adr:=pathListPtr[curTrailID];
  n:=pathStartEdge[curTrailID];
  px:=XEDGE[n]; py:=YEDGE[n];
  TLPosition:=0; ddx:=0; ddy:=0; loop:=false;
  repeat
    v:=peek(adr);
    case v of
      TR_JUMP:
        begin
          inc(adr);
          adr:=pointer(dpeek(adr));
          if (adr<pathListPtr[curTrailID]) or (adr>=pathListPtr[curTrailID+1]) then
          begin
            if (mode and PT_CALCLEN<>0) then TLPosition:=smallint(adr);
            break;
          end;
          if (mode=PT_CALCLEN) then break;
          if not loop then
            pathPattern:=loopPattern;
          loop:=true;
        end;
      TR_STOP:
        begin
          v:=0; break;
        end;
      TR_KILL:
        begin
          v:=3; break;
        end;
    else
      dx:=0; dy:=0;
      dx:=v shr 4;  if (dx and %1000)<>0 then dx:=dx or %11110000;
      dy:=v and $F; if (dy and %1000)<>0 then dy:=dy or %11110000;
      inc(px,dx); inc(py,dy);
      if ((px<_LEFT) or (px>_RIGHT)) or ((py<_TOP) or (py>_BOTTOM)) then
      begin
        v:=2; break;
      end;

      if (mode and PT_UPDATEPATH<>0) then
      begin
        w:=pathPattern and 1;
        point(px,py,w);
        pathPattern:=pathPattern shr 1;
        if w<>0 then pathPattern:=pathPattern or $800000;
      end;

      if TLPosition=curTLPos then
      begin
        curPx:=px; curPy:=py;
        curDx:=dx; curDy:=dy;
      end;

      if (not loop) and (mode and PT_UPDATETIMELINE<>0) then
      begin
        TLPoint(byte(v<>0));
      end;

      inc(adr);
      inc(TLPosition);
    end;
  until false;

  if ((mode and PT_CALCLEN<>0)) then result:=TLPosition;

  if (mode and PT_UPDATETIMELINE)<>0 then
  begin
    while TLPosition<160 do
    begin
      TLPoint(v); inc(TLPosition);
    end;
    TLPtr:=pointer(TLBUF_ADDR);
    scr:=YSCR[56+48+15]+(XTIMELINE div 4);
    for i:=0 to 4 do
    begin
      move(TLPtr,scr,TIMELINE_SIZEB);
      inc(TLPtr,TIMELINE_SIZEB);
      inc(scr,40);
    end;
  end;

  // if (mode and PT_KEEPCURSOR=0) then setCursor(_ARROW);
end;

//
//
//

procedure updateCurrentTool();
begin
  curTool:=curZone-13;
  case curTool of
    TOOL_MOVEPOINT: scr:=@_TRAYRIGHT+36;
    TOOL_ADDPOINT : scr:=@_TRAYRIGHT+24;
    TOOL_DELPOINT : scr:=@_TRAYRIGHT+12;
  else
    // nothingclear
    fillchar(Pointer(PMG_ADDR+$380+23),50,$3F);
    exit;
  end;
  move(scr,Pointer(PMG_ADDR+$380+24),48);
end;
