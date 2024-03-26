procedure clearSection();
begin
  wait(1);
  for i:=0 to 3 do HPOSM[i]:=0;
  HPOSP[3]:=0;
  for i:=14 to 31 do clearZone(i);
  for i:=56 to 56+47 do fillchar(Pointer(word(YSCR[i])+3),17,0);
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

//

function updatePathSection():boolean;
begin
  // if curSection=(selZone-10) then exit(false);
  result:=true;
  // curSection:=selZone-10;
  KEYB:=$FF;  // Prevent a previous key click from overriding
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

procedure updateStartPoint();
begin
  HPOSP[3]:=180;
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

//

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

//

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

//

procedure updateCurrentTool();
begin
  HPOSP[3]:=180;
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
