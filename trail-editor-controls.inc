procedure updatePathSection();
begin
  case szone of
  11: begin // Path Begin
        move(_ISELECT4+24,Pointer(PMG_ADDR+$300+24),48);
      end;
  12: begin // Inner Path
        move(_ISELECT4+12,Pointer(PMG_ADDR+$300+24),48);
      end;
  13: begin // Path Finish
        move(_ISELECT4+ 0,Pointer(PMG_ADDR+$300+24),48);
      end;
  end;
end;