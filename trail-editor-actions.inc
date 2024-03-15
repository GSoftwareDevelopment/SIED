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
  showTrailSelector();
end;

procedure doBack2PathBegin();
begin
  sZone:=11; doSwitchPathSection();
end;

procedure inGameScreen();
begin
  __invertZone:=false;
  if ((nZone=ZONE_VIEWPORT) and (oZone<>ZONE_VIEWPORT)) then // cursor enter zone
    setCursor(_CROSS)
  else if ((nZone<>ZONE_VIEWPORT) and (oZone=ZONE_VIEWPORT)) then // cursor leave zone
    setCursor(_ARROW);
end;

procedure inTimeLine();
begin
  __invertZone:=false;
  if ((nZone=ZONE_TIMELINE) and (oZone<>ZONE_TIMELINE)) then // cursor enter zone
    setCursor(@_TLSELECT)
  else if ((nZone<>ZONE_TIMELINE) and (oZone=ZONE_TIMELINE)) then // cursor leave zone
    setCursor(_ARROW);
end;

procedure updateTimeLineTime();
var
  min,sec,frm,bcd:Byte;
  s:string[8];

  function BIN2BCD(bin:Byte):Byte; Assembler;
  asm
    lda #0
    sta result+0

loop8Bits:
    ldy #{sed}
    asl bin
    adc result+0
    sta result+0
    iny
    bne loop8Bits+1
    cld
  end;

begin
  frm:=curTLPos mod FPS;
  sec:=curTLPos div FPS;
  min:=sec div 60;
  // setScreenWidth(40);
  for i:=56+63 to 56+67 do fillchar(YSCR[i],8,0);
  s:='  :  .  ';
  bcd:=bin2bcd(min);
  s[1]:=char(48+bcd shr 4);
  s[2]:=char(48+bcd and $f);
  bcd:=bin2bcd(sec);
  s[4]:=char(48+bcd shr 4);
  s[5]:=char(48+bcd and $f);
  bcd:=bin2bcd(frm);
  s[7]:=char(48+bcd shr 4);
  s[8]:=char(48+bcd and $f);
  putTextC(0,63,s);
end;

procedure doSetTimeLinePos();
begin
  curTLPos:=_mx-48-XTIMELINE-TLShift;
  if curTLPos>curTrailLength then curTLPos:=curTrailLength;
  HPOSM[0]:=48+XTIMELINE+curTLPos-TLShift;
  updateTimeLineTime();
end;

procedure doTLPrevEvent();
begin
  if curTLPos>0 then dec(curTLPos);
  HPOSM[0]:=48+XTIMELINE+curTLPos-TLShift;
  updateTimeLineTime();
  if oZone=3 then begin setControl(-1); oZone:=-1; end;
end;

procedure doTLNextEvent();
begin
  if curTLPos<curTrailLength then inc(curTLPos);
  HPOSM[0]:=48+XTIMELINE+curTLPos-TLShift;
  updateTimeLineTime();
  if oZone=6 then begin setControl(-1); oZone:=-1; end;
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
  activePage:=0;
  setScreenWidth(16);
  putSpriteXOR(@_OBJECT,8+curPX,4+curPY,1,7);
  activePage:=1;
  updateGameScreen();
  animTimer:=setTimer(PATH_ANIM_TIME,@onTimerAnim);
end;

procedure doSwitchPathSection();
begin
  if curSection=(sZone-10) then exit;
  clearSection();
  curSection:=sZone-10;
  updatePathSection();
  addZoneN(ZONE_VIEWPORT,XVIEWPORT div 4,0,26,48,@nullProc);
  _mZoneInCall[ZONE_VIEWPORT]:=@inGameScreen;
  clearGameScreen();
  clearTimer(animTimer);
  updateTimeLineTime();
  case curSection of
    0,1: showPathBegin();
    2: showPathInner();
    3: begin // Path Finish
        end;
  end;
end;