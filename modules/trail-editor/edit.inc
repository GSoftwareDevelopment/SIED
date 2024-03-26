procedure doExitEditMode(); Keep;
begin
  if cursorInViewPort then
  begin
    cursorInViewPort:=false;
    setCursor(_ARROW);
    curZone:=17; updateCurrentTool();
    registerShortcutKeys(@globalShortKeys,1); // register only ESC Event
  end;
end;

procedure inGameScreen();
begin
  allowInverZone:=false;
  if ((nowZone<>ZONE_VIEWPORT) and (oldZone=ZONE_VIEWPORT)) then // cursor leave zone
    doExitEditMode();
end;

procedure enterEditPath(n:shortint); Keep;
begin
  if (selZone<14) or (selZone>16) then curZone:=n;
  cursorInViewPort:=true;
  updateCurrentTool();
  unregisterToolShortKeys();
  setCursorPos(XVIEWPORT+8+curPX,curPY);
  curZone:=30; // if (nowZone=oldZone) then begin curZone:=30; end;
  registerToolShortKeys(@editModeShortKeys,SK_MOVEPOINT_COUNT);
end;

procedure doMovePoint(); Keep;
begin
  if curTool<>TOOL_MOVEPOINT then
    enterEditPath(14)
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
end;

procedure doAddPoint();
begin
  if curTool<>TOOL_ADDPOINT then
    enterEditPath(15)
  else
  begin
    case KEYB and $3f of
      k_INSERT: addTrailPoint(pathListPTr[curTrailID]+curTLPos);
    end;
  end;
end;

procedure doDeletePoint();
begin
  if curTool<>TOOL_DELPOINT then
    enterEditPath(16)
  else
  begin
    case KEYB and $3F of
      k_DELETE: delTrailPoint(pathListPtr[curTrailID]+curTLPos);
    end;
  end;
end;