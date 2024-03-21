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
  sZone:=11; doSwitchPathSection();
end;

procedure inGameScreen();
begin
  allowInverZone:=false;
  if ((nZone<>ZONE_VIEWPORT) and (oZone=ZONE_VIEWPORT)) then // cursor leave zone
  begin
    cursorInViewPort:=false;
    setCursor(_ARROW);
    curTool:=0; updateCurrentTool();
  end;
end;

procedure inTimeLine();
begin
  allowInverZone:=false;
  if ((nZone=ZONE_TIMELINE) and (oZone<>ZONE_TIMELINE)) then // cursor enter zone
    setCursor(@_TLSELECT)
  else if ((nZone<>ZONE_TIMELINE) and (oZone=ZONE_TIMELINE)) then // cursor leave zone
    setCursor(_ARROW);
end;

procedure doSetTimeLinePos();
begin
  curTLPos:=_mx-48-XTIMELINE-TLShift;
  if curTLPos>curTrailLength then curTLPos:=curTrailLength;
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
  if oZone=3 then
  begin setControl(-1); oZone:=-1; end;
end;

procedure doTLNextEvent(); Keep;
begin
  if (_modKey=0) then
  begin
    if curTLPos<curTrailLength then inc(curTLPos);
  end
  else
  if (_modKey=KMOD_SHIFT) then
  begin
    if (curTLPos+FPS)<curTrailLength then inc(curTLPos,FPS) else curTLPos:=curTrailLength;
  end
  else
  if _modKey=KMOD_CTRL then curTLPos:=curTrailLength;
  HPOSM[0]:=48+XTIMELINE+curTLPos-TLShift;
  updateTimeLineTime();
  if oZone=6 then
  begin setControl(-1); oZone:=-1; end;
end;

procedure doSetBeginEdge();
begin
  i:=sZone-ZONE_EDGE;
  pathStartEdge[curTrailID]:=i;
  pathStartEdgeShift[curTrailID]:=0;
  curSection:=0; sZone:=11; doSwitchPathSection();
  curTrailLength:=processTrail(PT_CALCLEN+PT_UPDATETIMELINE);
end;

procedure doRedefineStartPoint();
begin
  sZone:=10; doSwitchPathSection();
end;

CONST
  _OBJECT:Array[0..6] of byte = ( $F8,$F0,$F0,$F8,$9C,$08,$00 );

procedure onTimerAnim();
var
  b:byte=0;

begin
  inc(b); if b>5 then b:=0;
  loopPattern:=ANIM[b];
  processTrail(PT_KEEPCURSOR+PT_UPDATEPATH);
  // setCursorPos(XVIEWPORT+8+curPX,curPY);
  if not cursorInViewport then
  begin
    activePage:=0;
    setScreenWidth(16);
    putSpriteXOR(@_OBJECT,8+curPX,4+curPY,1,7);
    activePage:=1;
  end;
  updateGameScreen();
  updateStepInfo();
  animTimer:=setTimer(PATH_ANIM_TIME,@onTimerAnim);
end;

procedure doSwitchPathSection();
begin
  if curSection=(sZone-10) then exit;
  KEYB:=$FF;  // Prevent a previous key click from overriding
  clearSection();
  curSection:=sZone-10;
  unregisterShortcutKeys(@innerPathShortKeys,SK_INNER_COUNT);
  updatePathSection();
  clearZone(ZONE_VIEWPORT);
  clearGameScreen();
  clearTimer(animTimer);
  updateTimeLineTime();
  case curSection of
    0,1: showPathBegin();
    2: showPathInner();
    3: showPathFinish();
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
  if (_modKey=(KMOD_CTRL+KMOD_SHIFT)) then doBack2Selector();
  if cursorInViewPort then
  begin
    cursorInViewPort:=false;
    setCursor(_ARROW);
    curTool:=0; updateCurrentTool();
  end;
end;

procedure doEditPath(n:shortint); Keep;
begin
  if (sZone<14) or (sZone>16) then cZone:=n;
  cursorInViewPort:=true;
  setCursor(_CROSS);
  setCursorPos(XVIEWPORT+8+curPX,curPY);
  updateCurrentTool();
end;

procedure doMovePoint();
begin
  doEditPath(14);
end;

procedure doAddPoint();
begin
  doEditPath(15);
end;

procedure doDeletePoint();
begin
  doEditPath(16);
end;