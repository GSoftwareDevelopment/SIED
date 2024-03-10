procedure doBack2Selector();
begin
  fillchar(YSCR[56+48],800,0);
  fillchar(Pointer(PMG_ADDR+$300+23),50,$FF);
  _mzoneActive[1]:=true;
  setIcon(1);
  _mzoneActive[1]:=false;
  setControls(0);
  clearStatus();
  clearWorkarea();
  showTrailSelector();
end;

procedure doSwitchPathSection();
begin
  updatePathSection();
  case szone of
    11: begin // Path Begin
          // doEdit();
        end;
    12: begin // Inner Path
          // doName();
        end;
    13: begin // Path Finish
          // doDelete();
        end;
  end;
end;