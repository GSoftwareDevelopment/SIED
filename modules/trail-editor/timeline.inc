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
  if curTLPos>totalTrailLength then curTLPos:=totalTrailLength;
  updateTimeLineTime();
end;

procedure doTLPrevEvent(); Keep;
begin
  if curSection<>2 then exit;
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
  updateTimeLineTime();
  if curZone=3 then
    setControl(-1)
end;

procedure doTLNextEvent(); Keep;
begin
  if curSection<>2 then exit;
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
  updateTimeLineTime();
  if curZone=6 then
    setControl(-1)
end;
