procedure doBack2Selector();
begin
  fillchar(YSCR[56+48],800,0);
  fillchar(Pointer(PMG_ADDR+$300+23),50,$FF);
  HPOSM[2]:=0; HPOSM[3]:=0;
  _mzoneActive[1]:=true;
  setIcon(1);
  _mzoneActive[1]:=false;
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
  if ((nZone=31) and (oZone<>31)) then // cursor enter zone
    setCursor(_CROSS)
  else if ((nZone<>31) and (oZone=31)) then // cursor leave zone
    setCursor(_ARROW);
end;

procedure doSetBeginEdge();
begin
  i:=sZone-23;
  pathStartEdge[curTrailID]:=i;
  pathStartEdgeShift[curTrailID]:=0;
  curSection:=0; sZone:=11; doSwitchPathSection();
end;

procedure doRedefineStartPoint();
begin
  sZone:=10; doSwitchPathSection();
end;

procedure doSwitchPathSection();

  procedure setGameScreenZone();
  begin
    addZoneN(31,16,0,24,48,@nullProc);
    _mzoneInPROC[31]:=@inGameScreen;
  end;

begin
  if curSection=(sZone-10) then exit;
  clearSection();
  curSection:=sZone-10;
  updatePathSection();
  setGameScreenZone();
  case curSection of
    0,1: begin // Path Begin
          updateGameScreen();
          updateStartPoint();
          if (curSection=0) or (pathStartEdge[curTrailID]=-128) then
          begin
            addCorrners(@doSetBeginEdge);
            putText(20,30,'PLEASE DEFINE IT');
            putText(21,36,'BY CHOICE EDGE')
          end
          else
            addButton(21,36,'REDEFINE IT...',@doRedefineStartPoint);
        end;
    2: begin // Inner Path
          updateGameScreen();
        end;
    3: begin // Path Finish
        end;
  end;
end;