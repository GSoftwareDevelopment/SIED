procedure setModule();
var
  ncm:Shortint absolute $50; // alias to `szone` from interface

begin
  if ncm=curModule then exit;
  curModule:=ncm;

  for i:=3 to 9 do clearZone(i);
  _mzoneActive[0]:=true;
  _mzoneActive[1]:=true;
  _mzoneActive[2]:=true;
  fillchar(YSCR[56+48],600,0);
  HPOSP[2]:=0;
  setIcon(ncm);
  clearStatus();
  clearWorkarea();
  _mzoneActive[0]:=(ncm<>0);
  _mzoneActive[1]:=(ncm<>1);
  _mzoneActive[2]:=(ncm<>2);
  case ncm of
    0:showDiskDirectory();
    1:showPathEditor();
    2:showScenarioEditor();
  end;
End;

procedure initModules();
begin
  setZone(0,false,28,49,3,15,@setModule);
  setZone(1,false,32,49,3,15,@setModule);
  setZone(2,false,36,49,3,15,@setModule);
  for i:=56+48+16 to 56+48+18 do fillchar(Pointer(word(YSCR[i])+8),32,%01010101);
  putTextC(0,63,'00:00.00');
  filemask:='*.*'#$9B;
  dev:='D:';
  fn:='NONAME';
End;