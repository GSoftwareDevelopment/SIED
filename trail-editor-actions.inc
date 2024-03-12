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
  szone:=11; doSwitchPathSection();
end;

procedure inGameScreen();
begin
  __invertZone:=false;
  if ((nZone>=23) and (oZone<23)) then // cursor enter zone
  begin
    setCursorShapeAnchor(3,3); setCursorShape(_CROSS);
  end
  else if ((nZone<23) and (oZone>=23)) then // cursor leave zone
  begin
    setCursorShapeAnchor(0,0); setCursorShape(_ARROW);
  end;
end;

procedure doSwitchPathSection();

  procedure setGameScreenZone();
  begin
    addZoneN(31,16,0,24,48,@nullProc);
    _mzoneInPROC[31]:=@inGameScreen;
  end;

begin
  if curSection=(szone-10) then exit;
  clearSection();
  curSection:=szone-10;
  updatePathSection();
  case curSection of
    1: begin // Path Begin
          updateGameScreen();
          putText(21,7,'SELECT A POINT');
          putText(22,13,'OF BEGINNING');
          fillchar(YSCR[56+19]+10,8,$FF);
          addButton(23,30,'EDGE', @doSwitchPathSection);
          addButton(29,30,'FREE', @doSwitchPathSection);
        end;
    2: begin // Inner Path
          updateGameScreen();
          setGameScreenZone();
        end;
    3: begin // Path Finish
        end;
    4: begin // Beginning: Edge
          updateGameScreen();
          setGameScreenZone();
          addCorrners(@inGameScreen);
          addButton(8,1,'<< BACK', @doBack2PathBegin);
          putText(23,22,'CHOICE EDGE');
      end;
    5: begin // Beginning: Free
          updateGameScreen();
          setGameScreenZone();
          addButton(8,1,'<< BACK', @doBack2PathBegin);
          putText(22,19,'SET POINT IN');
          putText(22,26,'GAME WORKAREA');
      end;
  end;
end;