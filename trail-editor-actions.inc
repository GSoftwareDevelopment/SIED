procedure doBack2Selector();
begin
  if (KEYB<>$FF) and (_modKey<>(KMOD_CTRL+KMOD_SHIFT)) then exit;
  clearTimer(animTimer);
  fillchar(YSCR[56+48],800,0);
  fillchar(Pointer(PMG_ADDR+$300+23),50,$FF);
  for i:=0 to 3 do HPOSM[i]:=0;
  HPOSP[3]:=0;
  for n:=0 to 2 do zoneActive[n]:=true;
  setIcon(1);
  zoneActive[1]:=false;
  setControls(0);
  clearStatus();
  clearWorkarea();
  unregisterShortcutKeys(@globalShortKeys);
  unregisterShortcutKeys(@innerPathShortKeys);
  showTrailSelector();
end;

//

procedure doSetBeginEdge();
begin
  case KEYB of
    K_UP:     i:=0;
    K_LEFT:   i:=1;
    K_RIGHT:  i:=2;
    K_DOWN:   i:=3;
  else
    i:=selZone-14;
  end;

  pathStartEdge[curTrailID]:=i;
  updateStartPoint();
  trailLength:=processTrail(PT_CALCLEN+PT_UPDATETIMELINE);
end;

//

procedure onTimerAnim();
var
  b:byte=0;
  _x,_y:Byte;

begin
  inc(b); if b>5 then b:=0;
  loopPattern:=PATHANIM[b];
  processTrail(PT_UPDATEPATH);
  if not cursorInViewport then
  begin
    activePage:=0;
    setScreenWidth(16);
    case curSection of
      1: begin
        n:=pathStartEdge[curTrailID];
        if (n<>-128) then
        begin
          px:=_XEDGE[n];
          py:=_YEDGE[n];
          adr:=@_EDGES+ n*5;
        end;
      end;
      2: begin
          px:=8+curPX; py:=4+curPY; adr:=@_IPOINTARROW;
          if px>_RIGHT then begin dec(px,6); inc(adr,5); end;
          if py>_BOTTOM then begin dec(py,4); inc(adr,10); end;
        end;
    end;
    putSpriteXOR(adr,px,py,1,5);
    activePage:=1;
  end
  else
  begin
    if ((curTLPos>=trailLength) and (curTool<>TOOL_ADDPOINT)) or
       ((curTLPos>trailLength) and (curTool=TOOL_ADDPOINT)) then
      setCursor(@_STOP)
    else
      setCursor(_CROSS);

    _x:=_mx-56-XVIEWPORT; _y:=_my-24;
    if  ((curPX<>_curPX) or (curPY<>_curPY)) and
        ((curPX<>_x) or (curPY<>_y)) then
    begin
      setCursorPos(XVIEWPORT+8+curPX,curPY);
      curPX:=_x; curPY:=_y;
    end;
  end;
  updateGameScreen();
  updateStepInfo();
  animTimer:=setTimer(PATH_ANIM_REFRESH,@onTimerAnim);
end;

//

function doSwitchPathSection():Boolean;
begin
  result:=false;
  if KEYB<>$FF then
  begin
    case byte(KEYB+_modKey) of
      k_B + KMOD_CTRL: curSection:=1;
      k_I + KMOD_CTRL: curSection:=2;
      k_F + KMOD_CTRL: curSection:=3;
    else
      exit;
    end;
  end
  else
    curSection:=selZone-10;
  if not updatePathSection() then exit;
  clearSection();
  unregisterShortcutKeys(@beginPathShortKeys);
  unregisterShortcutKeys(@innerPathShortKeys);
  unregisterToolShortKeys();
  clearZone(ZONE_VIEWPORT);
  clearTimer(animTimer);
  updateTimeLineTime();
  case curSection of
    0: begin end; // TODO: Editor config - not implemented yet
    1: showPathBegin();
    2: showPathInner();
    3: showPathFinish();
  end;
  if curSection<>0 then
  begin
    trailPattern:=%111111111111111111111111;
    loopPattern:=%100100100100100100100100;
    trailLength:=processTrail(PT_CALCLEN);
    totalTrailLength:=processTrail(PT_CALCLEN+PT_UPDATETIMELINE);
    onTimerAnim();
  end;
  registerShortcutKeys(@globalShortKeys);
  result:=true;
end;
