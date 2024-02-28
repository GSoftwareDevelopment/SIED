procedure showPathEditor();
begin
  if curModule=1 then exit;
  setModule(1);

   addZone(3,1,YCONTROLS,3,7,@nullProc);  // previous
  addZoneH(4,@nullProc);  // play
  addZoneH(5,@nullProc);  // stop
  addZoneH(6,@nullProc);  // next

  addZone(7,15,YCONTROLS,3,7,@nullProc);  // insert
  addZoneH(8,@nullProc);  // delete

  setControl(-1);
  HPOSP[2]:=44; PCOL[2]:=$E6; SIZEP[2]:=%11;
  SetScreenWidth(20);
  putImage(@_IPATH,0,0,3,48);

  addZone(0,0,3,12,@nullProc); // list
  addZoneV(@nullProc); // pen
  addZoneV(@nullProc); // path 1
  addzoneV(@nullProc); // sprite
  addZone(3,0,3,12,@nullProc);  // arrow
  addZoneV(@nullProc); // line
  addZoneV(@nullProc); // path 2
End;
