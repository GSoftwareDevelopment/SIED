procedure doExitEditMode(); Keep;
begin
  if cursorInViewPort then
  begin
    cursorInViewPort:=false;
    setCursor(_ARROW);
    updateCurrentTool(-1);
    registerShortcutKeys(@globalShortKeys,1); // register only ESC Event
  end;
end;

procedure inGameScreen();
begin
  allowInverZone:=false;
  if ((nowZone<>ZONE_VIEWPORT) and (oldZone=ZONE_VIEWPORT)) then // cursor leave zone
    doExitEditMode();
end;

function enterEditPath(nTool:shortint):Boolean;
begin
  if curTool=nTool then exit(false);
  result:=true;
  cursorInViewPort:=true;
  updateCurrentTool(nTool);
  unregisterToolShortKeys();
  setCursorPos(XVIEWPORT+8+curPX,curPY);
  curZone:=30;
  registerToolShortKeys(@editModeShortKeys,SK_MOVEPOINT_COUNT);
end;

procedure doMovePoint(); Keep;
begin
  if (curTLPos>0) and (curTLPos>=trailLength) then exit;
  if enterEditPath(TOOL_MOVEPOINT) then exit;
  case KEYB of
    K_UP:     if curDY>-7 then dec(curDY);
    K_DOWN:   if curDY<7  then inc(curDY);
    K_LEFT:   if curDX>-7 then dec(curDX);
    K_RIGHT:  if curDX<7  then inc(curDX);
  end;
  if curTLPos<=trailLength then
  begin
    adr:=pathListPtr[curTrailID]+curTLPos;
    v:=peek(adr);
    if (v<>TR_JUMP) and (v<>TR_STOP) and (v<>TR_KILL) then
    begin
      v:=(curDX shl 4) + (curDY and $F);
      poke(adr,v);
      trailLength:=processTrail(PT_CALCLEN);
      totalTrailLength:=processTrail(PT_CALCLEN+PT_UPDATETIMELINE);
    end;
  end;
end;

procedure doAddPoint();
begin
  if (curTLPos>trailLength) then exit;
  if enterEditPath(TOOL_ADDPOINT) then exit;
  case KEYB of
    k_INSERT: addTrailPoint(curTLPos);
  end;
end;

procedure doDeletePoint();
begin
  if (curTLPos>0) and (curTLPos>=trailLength) then exit;
  if enterEditPath(TOOL_DELPOINT) then exit;
  case KEYB of
    k_DELETE: delTrailPoint(curTLPos);
  end;
end;