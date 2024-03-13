procedure clearSection();
begin
  wait(1);
  HPOSM[2]:=0; HPOSM[3]:=0;
  for i:=14 to 31 do clearZone(i);
  for i:=56 to 56+47 do fillchar(Pointer(word(YSCR[i])+3),17,0);
end;

procedure updateGameScreen();
begin
  HPOSM[2]:=112; HPOSM[3]:=200;
  asm
  lline:
    ldy #11
  lcpy:
    :48 mva SCREEN_ADDR+(#*16),y EDITOR_ADDR+(8+#*20),y
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
var
  p:pointer;
  x,y:Byte;

begin
  for i:=0 to 3 do
  begin
    p:=@_CORNERS+ i*4;
    x:=CORNERSX[i];
    y:=CORNERSY[i];
    putSprite(p, x, y,1,4);
    x:=x div 4;
    addZoneN(23+i,x,y,2,4,prc);
  end;
end;

procedure updateStartPoint();
const
  CTOP   = 8;
  CLINE1 = 17;
  CLEFT  = 22;

var
  px,py:Shortint;

begin
  putText(CLEFT-1,CTOP,'CURRENT START:');
  fillchar(YSCR[56+CTOP+6]+10,8,$FF);
  blank(CLEFT,CLINE1,6,11);
  n:=pathStartEdge[curTrailID];
  if (n<>-128) then
  begin
    i:=Length(EDGENAME[n]);
    px:=CLEFT+6-(i shr 1);
    putText(px,CLINE1,EDGENAME[n]);
    invert(px-1,CLINE1-1,i+1,6);
    px:=CORNERSX[n];
    py:=CORNERSY[n];
    putSprite((@_CORNERS+ n*4), px, py,1,4);
  end;
end;
