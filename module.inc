procedure setModule(cm:Shortint);
begin
  curModule:=cm;
  for i:=3 to 9 do clearZone(i);
  _mzoneActive[0]:=true;
  _mzoneActive[1]:=true;
  _mzoneActive[2]:=true;
  fillchar(YSCR[56+48],600,0);
  HPOSP[2]:=0;
  setIcon(cm);
  clearStatus();
  clearWorkarea();
  _mzoneActive[0]:=(cm<>0);
  _mzoneActive[1]:=(cm<>1);
  _mzoneActive[2]:=(cm<>2);
End;

procedure initModules();
begin
  setZone(0,false,28,49,3,15,@showDiskDirectory);
  setZone(1,false,32,49,3,15,@showPathEditor);
  setZone(2,false,36,49,3,15,@showScenarioEditor);
  for i:=56+48+16 to 56+48+18 do fillchar(Pointer(word(YSCR[i])+8),32,%01010101);
  putTextC(0,63,'00:00.00');
  filemask:='*.*'#$9B;
  dev:='D:';
  fn:='NONAME';
End;