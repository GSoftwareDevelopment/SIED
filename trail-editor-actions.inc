procedure doBack2Selector();
begin
  clearTimer(animTimer);
  fillchar(YSCR[56+48],800,0);
  fillchar(Pointer(PMG_ADDR+$300+23),50,$FF);
  for i:=0 to 3 do HPOSM[i]:=0;
  HPOSP[3]:=0;
  _mZoneActive[1]:=true;
  setIcon(1);
  _mZoneActive[1]:=false;
  setControls(0);
  clearStatus();
  clearWorkarea();
  unregisterShortcutKeys(@globalShortKeys,SK_GLOBAL_COUNT);
  unregisterShortcutKeys(@innerPathShortKeys,SK_INNER_COUNT);
  showTrailSelector();
end;

procedure doBack2PathBegin();
begin
  selZone:=11; doSwitchPathSection();
end;

procedure inGameScreen();
begin
  allowInverZone:=false;
  if ((nowZone<>ZONE_VIEWPORT) and (oldZone=ZONE_VIEWPORT)) then // cursor leave zone
  begin
    cursorInViewPort:=false;
    setCursor(_ARROW);
    curZone:=-1; updateCurrentTool();
  end;
end;

procedure inTimeLine();
begin
  allowInverZone:=false;
  if ((nowZone=ZONE_TIMELINE) and (oldZone<>ZONE_TIMELINE)) then // cursor enter zone
    setCursor(@_TLSELECT)
  else if ((nowZone<>ZONE_TIMELINE) and (oldZone=ZONE_TIMELINE)) then // cursor leave zone
    setCursor(_ARROW);
end;

procedure doSetTimeLinePos();
begin
  curTLPos:=_mx-48-XTIMELINE-TLShift;
  if curTLPos>trailLength then curTLPos:=trailLength;
  HPOSM[0]:=48+XTIMELINE+curTLPos-TLShift;
  updateTimeLineTime();
end;

procedure doTLPrevEvent(); Keep;
begin
  if (_modKey=0) then
  begin
    if curTLPos>0 then dec(curTLPos)
  end
  else
  if (_modKey=KMOD_SHIFT) then
  begin
    if (curTLPos-FPS)>0 then dec(curTLPos,FPS) else curTLPos:=0;
  end
  else
  if _modKey=KMOD_CTRL then curTLPos:=0;
  HPOSM[0]:=48+XTIMELINE+curTLPos-TLShift;
  updateTimeLineTime();
  if curZone=nowZone then begin setControl(-1); oldZone:=-1; end;
end;

procedure doTLNextEvent(); Keep;
begin
  if (_modKey=0) then
  begin
    if curTLPos<totalTrailLength then inc(curTLPos);
  end
  else
  if (_modKey=KMOD_SHIFT) then
  begin
    if (curTLPos+FPS)<totalTrailLength then inc(curTLPos,FPS) else curTLPos:=totalTrailLength;
  end
  else
  if _modKey=KMOD_CTRL then curTLPos:=totalTrailLength;
  HPOSM[0]:=48+XTIMELINE+curTLPos-TLShift;
  updateTimeLineTime();
  if curZone=nowZone then begin setControl(-1); oldZone:=-1; end;
end;

procedure doSetBeginEdge();
begin
  i:=selZone-14;
  pathStartEdge[curTrailID]:=i;
  updateStartPoint();
  trailLength:=processTrail(PT_CALCLEN+PT_UPDATETIMELINE);
end;

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
          if px>_RIGHT then begin dec(px,7); inc(adr,5); end;
          if py>_BOTTOM then begin dec(py,4); inc(adr,10); end;
        end;
    end;
    putSpriteXOR(adr,px,py,1,5);
    activePage:=1;
  end
  else
  begin
    if (curTLPos>=trailLength) and (curTool<>TOOL_ADDPOINT) then
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

procedure doSwitchPathSection();
begin
  if curSection=(selZone-10) then exit;
  KEYB:=$FF;  // Prevent a previous key click from overriding
  clearSection();
  curSection:=selZone-10;
  unregisterShortcutKeys(@innerPathShortKeys,SK_INNER_COUNT);
  updatePathSection();
  clearZone(ZONE_VIEWPORT);
  clearGameScreen();
  clearTimer(animTimer);
  updateTimeLineTime();
  case curSection of
    1: showPathBegin();
    2: showPathInner();
    3: showPathFinish();
  end;
  if curSection<>3 then
  begin
    trailPattern:=%111111111111111111111111;
    loopPattern:=%100100100100100100100100;
    trailLength:=processTrail(PT_CALCLEN);
    totalTrailLength:=processTrail(PT_CALCLEN+PT_UPDATETIMELINE);
    onTimerAnim();
  end;
end;

procedure keySwitchPathSection(n:shortint); Keep;
begin
  if _modKey<>KMOD_CTRL then exit;
  setCursorInZone(n);
  doSwitchPathSection();
end;

procedure keySwitch2BeginPath(); Keep;
begin
  keySwitchPathSection(11);
end;
procedure keySwitch2InnerPath(); Keep;
begin
  keySwitchPathSection(12);
end;
procedure keySwitch2FinishPath(); Keep;
begin
  keySwitchPathSection(13);
end;

//
//
//

procedure doExitEditMode(); Keep;
begin
  if (_modKey=(KMOD_CTRL+KMOD_SHIFT)) then
  begin
    doBack2Selector();
  end
  else
    if cursorInViewPort then
    begin
      cursorInViewPort:=false;
      setCursor(_ARROW);
      curZone:=17; updateCurrentTool();
    end;
end;

procedure enterEditPath(n:shortint); Keep;
begin
  if (selZone<14) or (selZone>16) then curZone:=n;
  cursorInViewPort:=true;
  updateCurrentTool();
  unregisterToolShortKeys();
  setCursorPos(XVIEWPORT+8+curPX,curPY);
  curZone:=30; // if (nowZone=oldZone) then begin curZone:=30; end;
end;

procedure doMovePoint(); Keep;
var
  b:Byte;

begin
  if curTool<>TOOL_MOVEPOINT then
  begin
    enterEditPath(14); registerToolShortKeys(@movePointShortKeys,SK_MOVEPOINT_COUNT);
  end
  else
  begin
    case KEYB of
      K_UP:     if curDY>-7 then dec(curDY);
      K_DOWN:   if curDY<7  then inc(curDY);
      K_LEFT:   if curDX>-7 then dec(curDX);
      K_RIGHT:  if curDX<7  then inc(curDX);
    end;
    if curTLPos<=trailLength then
    begin
      adr:=pathListPtr[curTrailID]+curTLPos;
      b:=peek(adr);
      if (b<>TR_JUMP) and (b<>TR_STOP) and (b<>TR_KILL) then
      begin
        b:=(curDX shl 4) + (curDY and $F);
        poke(adr,b);
        trailLength:=processTrail(PT_CALCLEN);
        totalTrailLength:=processTrail(PT_CALCLEN+PT_UPDATETIMELINE);
      end;
    end;
  end;
end;

procedure doAddPoint();
begin
  enterEditPath(15);
end;

procedure doDeletePoint();
begin
  enterEditPath(16);
end;