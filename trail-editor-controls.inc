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

procedure updatePathSection();
begin
  case curSection of
    0,1: scr:=@_ISELECT4+24; // Path Begin
    2: scr:=@_ISELECT4+12; // Inner Path
    3: scr:=@_ISELECT4+ 0; // Path Finish
  else
    // nothing
    fillchar(Pointer(PMG_ADDR+$300+23),50,$FF);
    exit;
  end;
  move(scr,Pointer(PMG_ADDR+$300+24),48);
end;

procedure addEdges(prc:TZoneProc);
begin
  SetScreenWidth(20);
  for i:=0 to 3 do
  begin
    adr:=@_CORNERS+ (i*5);
    px:=XCORNERS[i];
    py:=YCORNERS[i];
    putSprite(adr, px, py, 1, 5);
    px:=byte(px) div 4;
    addZoneN(ZONE_EDGE+i, px, py, 2, 5,prc);
  end;
end;

procedure updateStartPoint(mode:Byte);
const
  CTOP   = 8;
  CLINE1 = 17;
  CLEFT  = (XVIEWPORT div 4)+7;

begin
  if mode=SP_DESCRIPTION then
  begin
    setScreenWidth(20);
    putText(CLEFT-1,CTOP,'CURRENT START:');
    fillchar(YSCR[56+CTOP+6]+(XVIEWPORT div 8)+2,9,$FF);
  end;
  n:=pathStartEdge[curTrailID];
  if (n<>-128) then
  begin
    if mode=SP_DESCRIPTION then
    begin
      i:=Length(EDGENAME[n]);
      px:=CLEFT+5-(i shr 1);
      blank(px-1,CLINE1-1,i+1,6);
      putText(px,CLINE1,EDGENAME[n]);
      invert(px-1,CLINE1-1,i+1,6);
    end;
    px:=XCORNERS[n];
    py:=YCORNERS[n];
    putSprite((@_CORNERS+ n*5), px, py,1,5);
  end;
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
    TLPtr:=YSCR[56+48+15]+(XTIMELINE div 4);
    fillchar(TLPtr,TIMELINELENGTH div 4,$aa);
    inc(TLPtr,40);
    for i:=0 to 2 do
    begin
      fillchar(TLPtr,TIMELINELENGTH div 4,_LOOP[i]);
      inc(TLPtr,40);
    end;
    fillchar(TLPtr,TIMELINELENGTH div 4,$aa);
  end;
  scr:=YSCR[56+48+16]+8;
  if (mode and PT_KEEPCURSOR=0) then setCursor(_WAIT);
  adr:=pathListPtr[curTrailID];
  n:=pathStartEdge[curTrailID];
  px:=XEDGE[n];
  py:=YEDGE[n];
  TLPosition:=0; ddx:=0; ddy:=0; loop:=false;
  repeat
    v:=peek(adr);
    case v of
      $80: // TR_JUMP
        begin
          inc(adr);
          adr:=pointer(dpeek(adr));
          if (adr<pathListPtr[curTrailID]) or (adr>=pathListPtr[curTrailID+1]) then
          begin
            if (mode and PT_CALCLEN<>0) then TLPosition:=smallint(adr);
            break;
          end;
          if loop then
          begin
            // if (ddx=0) and (ddy=0) then
            // begin
            //   if (mode and PT_CALCLEN<>0) then TLPosition:=-TLPosition;
            //   break;
            // end
            // else
              if (mode=PT_CALCLEN) then break;
          end
          else
            pathPattern:=loopPattern;
          loop:=true;
        end;
      $88: // TR_STOP
        begin
          v:=0; break;
        end;
      $8F: // TR_KILL
        begin
          v:=3; break;
        end;
    else
      if TLPosition=curTLPos then
      begin
        curPx:=px; curPy:=py;
        curDx:=0;  curDy:=0;
      end;
      if (mode and PT_UPDATEPATH<>0) then
      begin
        w:=pathPattern and 1;
        point(px,py,byte(w<>0));
        pathPattern:=pathPattern shr 1;
        if w<>0 then pathPattern:=pathPattern or $800000;
      end;
      if v<>0 then
      begin
        if (mode and PT_UPDATETIMELINE<>0) and not loop then TLPoint(1);
        dx:=v shr 4;  if (dx and %1000)<>0 then dx:=dx or %11110000;
        dy:=v and $F; if (dy and %1000)<>0 then dy:=dy or %11110000;
        if TLPosition=curTLPos then
        begin
          curDx:=dx; curDy:=dy;
        end;
        inc(px,dx); inc(py,dy);
        if ((px<_LEFT) or (px>_RIGHT)) or ((py<_TOP) or (py>_BOTTOM)) then
        begin
          v:=2; break;
        end;
      end
      else
        if (mode and PT_UPDATETIMELINE<>0) and not loop then TLPoint(0);
      inc(adr);
      inc(TLPosition);
    end;
  until false;

  if ((mode and PT_CALCLEN<>0)) then result:=TLPosition;

  if (mode and PT_UPDATETIMELINE)<>0 then
    while TLPosition<160 do
    begin
      inc(TLPosition); TLPoint(v);
    end;

  if (mode and PT_KEEPCURSOR=0) then setCursor(_ARROW);
end;