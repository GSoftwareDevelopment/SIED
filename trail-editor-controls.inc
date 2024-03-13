procedure clearSection();
begin
  wait(1);
  HPOSM[2]:=0; HPOSM[3]:=0;
  for i:=14 to 31 do clearZone(i);
  for i:=56 to 56+47 do fillchar(Pointer(word(YSCR[i])+3),17,0);
end;

procedure updateGameScreen();
begin
  HPOSM[2]:=48+XVIEWPORT; HPOSM[3]:=48+88+XVIEWPORT;
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
    0,1,4,5: scr:=@_ISELECT4+24; // Path Begin
    2: scr:=@_ISELECT4+12; // Inner Path
    3: scr:=@_ISELECT4+ 0; // Path Finish
  else
    // nothing
    fillchar(Pointer(PMG_ADDR+$300+23),50,$FF);
    exit;
  end;
  move(scr,Pointer(PMG_ADDR+$300+24),48);
end;

procedure addCorrners(prc:TZoneProc);
begin
  for i:=0 to 3 do
  begin
    adr:=@_CORNERS+ i*4;
    px:=XCORNERS[i];
    py:=YCORNERS[i];
    putSprite(adr, px, py, 1, 4);
    px:=px div 4;
    addZoneN(23+i, px, py, 2, 4,prc);
  end;
end;

procedure updateStartPoint();
const
  CTOP   = 8;
  CLINE1 = 17;
  CLEFT  = (XVIEWPORT div 4)+6;

begin
  putText(CLEFT-1,CTOP,'CURRENT START:');
  fillchar(YSCR[56+CTOP+6]+(XVIEWPORT div 8)+2,8,$FF);
  // blank(CLEFT,CLINE1,6,11);
  n:=pathStartEdge[curTrailID];
  if (n<>-128) then
  begin
    i:=Length(EDGENAME[n]);
    px:=CLEFT+5-(i shr 1);
    blank(px-1,CLINE1-1,i+1,6);
    putText(px,CLINE1,EDGENAME[n]);
    invert(px-1,CLINE1-1,i+1,6);
    px:=XCORNERS[n];
    py:=YCORNERS[n];
    putSprite((@_CORNERS+ n*4), px, py,1,4);
  end;
end;

procedure point(x,y:Shortint; c:Byte);
var
  bit:byte;

begin
  bit:=x and 7; x:=(8+x) shr 3;
  scr:=YSCR[4+y]+x;
  bit:=pbit[bit];
  if c=0 then
    poke(scr,peek(scr) and not bit)
  else
    poke(scr,peek(scr) or bit);
end;

function processTrail(mode:Byte):Smallint;

  procedure TLPoint(c:Byte);
  var
    m,b:Byte;
    pos:Smallint;
    bit:Byte;

  begin
    if (mode and PT_UPDATETIMELINE=0) or loop then exit;
    pos:=TLPosition-TLShift;
    if (pos>=0) and (pos<(TIMELINELENGTH div 4)) then
    begin
      TLPtr:=YSCR[56+48+16]+8+pos shr 3;
      bit:=pos and 3;
      m:=P4Mask[bit]; if c>0 then b:=P4Color[c-1][bit] else b:=0;
      poke(TLPtr,(peek(TLPtr) and m) or b); inc(TLPtr,40);
      poke(TLPtr,(peek(TLPtr) and m) or b); inc(TLPtr,40);
      poke(TLPtr,(peek(TLPtr) and m) or b);
      if bit=3 then inc(TLPtr);
    end;
  end;

begin
  if (mode and PT_UPDATEPATH<>0) then
    fillchar(Pointer(SCREEN_ADDR),$380,0);
  ntm:=tm+1;
  pathPattern:=%11111111;
  if (mode and PT_UPDATETIMELINE<>0) then
  begin
    TLPtr:=YSCR[56+48+16]+8;
    for i:=0 to 2 do
    begin
      fillchar(TLPtr,TIMELINELENGTH div 4,_LOOP[i]);
      inc(TLPtr,40);
    end;
  end;
  scr:=YSCR[56+48+16]+8;
  setCursor(_WAIT);
  adr:=pathListPtr[curTrailID];
  n:=pathStartEdge[curTrailID];
  px:=XEDGE[n];
  py:=YEDGE[n];
  TLPosition:=0; ddx:=0; ddy:=0; loop:=false;
  repeat
    // if ((mode and PT_CALCLEN<>0)) and (tm=ntm) then
    // begin
    //   ntm:=tm+1;
    //   scr:=YSCR[56+48+16]+8;
    //   for i:=0 to 2 do
    //   begin
    //     fillchar(scr,TIMELINELENGTH div 4,_PROCESS[i]);
    //     inc(scr,40);
    //   end;
    //   asm
    //     ldy adr._PROCESS+0
    //     lda adr._PROCESS+1
    //     sta adr._PROCESS+0
    //     lda adr._PROCESS+2
    //     sta adr._PROCESS+1
    //     lda adr._PROCESS+3
    //     sta adr._PROCESS+2
    //     sty adr._PROCESS+3
    //   end;
    // end;

    v:=peek(adr);
    case v of
      $80: // TR_JUMP
        begin
          inc(adr);
          adr:=pointer(dpeek(adr));
          if (adr<pathListPtr[curTrailID]) or (adr>=pathListPtr[curTrailID+1]) then
          begin
            if (mode and PT_CALCLEN<>0) then result:=smallint(adr);
            break;
          end;
          if loop then
          begin
            if (ddx=0) and (ddy=0) then
            begin
              if (mode and PT_CALCLEN<>0) then result:=-TLPosition;
              break;
            end
            else
              if (mode=PT_CALCLEN) then
              begin
                result:=TLPosition;
                break;
              end;
          end;
          loop:=true;
          pathPattern:=%10101010;
        end;
      $88: // TR_STOP
        begin
          if (mode and PT_CALCLEN<>0) then result:=TLPosition;
          v:=3; break;
        end;
      $8F: // TR_KILL
        begin
          if (mode and PT_CALCLEN<>0) then result:=TLPosition;
          v:=0; break;
        end;
    else
      if v<>0 then
      begin
        TLPoint(1);
        dx:=v shr 4;  if (dx and %1000)<>0 then dx:=dx or %11110000;
        dy:=v and $F; if (dy and %1000)<>0 then dy:=dy or %11110000;
        inc(px,dx); inc(py,dy);
        if ((px<_LEFT) or (px>_RIGHT)) or ((py<_TOP) or (py>_BOTTOM)) then
        begin
          if ((mode and PT_CALCLEN<>0)) then result:=TLPosition;
          v:=0; break;
        end;
        if (mode and PT_UPDATEPATH<>0) then
        begin
          v:=pathPattern and $80;
          point(px,py,v);
          pathPattern:=pathPattern shl 1;
          if v<>0 then pathPattern:=pathPattern or 1;
        end;
        inc(ddx,dx); inc(ddy,dy);
      end
      else
        TLPoint(3);
      inc(adr);
      inc(TLPosition);
    end;
  until false;

  if (mode and PT_UPDATETIMELINE)=PT_UPDATETIMELINE then
    while TLPosition<160 do
    begin
      inc(TLPosition); TLPoint(v);
    end;

  if (mode and PT_UPDATEPATH<>0) then
    updateGameScreen();
  setCursor(_ARROW);
end;