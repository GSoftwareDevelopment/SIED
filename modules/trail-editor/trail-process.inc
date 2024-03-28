function processTrail(mode:Byte):Smallint;
var
  w,pathPattern:longint;

begin
  if (mode and PT_UPDATEPATH<>0) then
  begin
    fillchar(Pointer(SCREEN_ADDR),$380,0);
  end;

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
    while TLPosition<TIMELINELENGTH do
    begin
      TLPoint(v); inc(TLPosition);
    end;
    TLPtr:=pointer(TLBUF_ADDR);
    scr:=YSCR[56+48+14]+(XTIMELINE div 4);
    for i:=0 to 4 do
    begin
      move(TLPtr,scr,TIMELINE_SIZEB);
      inc(TLPtr,TIMELINE_SIZEB);
      inc(scr,40);
    end;
  end;
end;

procedure addTrailPoint(TLPos:SmallInt);
begin
  adr:=pathListPtr[curTrailID]+TLPos;
  move(adr,adr+1,$BFFF-word(adr));
  poke(adr,$00);
  trailLength:=processTrail(PT_CALCLEN);
  totalTrailLength:=processTrail(PT_CALCLEN+PT_UPDATETIMELINE);
end;

procedure delTrailPoint(TLPos:SmallInt);
begin
  adr:=pathListPtr[curTrailID]+TLPos;
  v:=peek(adr);
  if (v<>TR_JUMP) and (v<>TR_STOP) and (v<>TR_KILL) then
  begin
    move(adr+1,adr,$C000-word(adr));
    trailLength:=processTrail(PT_CALCLEN);
    totalTrailLength:=processTrail(PT_CALCLEN+PT_UPDATETIMELINE);
  end;
end;
