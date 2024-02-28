procedure showScenarioEditor();
begin
  if curModule=2 then exit;
  setModule(2);
   addZone(3,1,YCONTROLS,3,7,@nullProc);  // previous
  addZoneH(4,@nullProc);  // play
  addZoneH(5,@nullProc);  // stop
  addZoneH(6,@nullProc);  // next

  addZone(7,15,YCONTROLS,3,7,@nullProc);  // insert
  addZoneH(8,@nullProc);  // delete
  setControl(-1);
End;
