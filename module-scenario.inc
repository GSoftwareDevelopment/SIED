procedure showScenarioEditor();
begin
   addZoneN(3,1,YCONTROLS,3,7,@nullProc);  // previous
  addZoneHN(4,@nullProc);  // play
  addZoneHN(5,@nullProc);  // stop
  addZoneHN(6,@nullProc);  // next

   addZoneN(7,15,YCONTROLS,3,7,@nullProc);  // insert
  addZoneHN(8,@nullProc);  // delete
  setControl(-1);
End;
