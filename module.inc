{$I 'about.h.inc'}
{$I 'disk.h.inc'}
{$I 'pathed.h.inc'}
{$I 'scened.h.inc'}

var
  curModule:Byte = -128;
  moduleInitialized:Byte absolute $62;

procedure loadModule(FN:PString);
begin
  setCursorShapeAnchor(3,3); setCursorShape(_WAIT);
  setStatus('LOADING MODULE...');
  XIO($28, 2, 4, $00, FN);
  clearStatus();
  setCursorShapeAnchor(0,0); setCursorShape(_ARROW);
  clearAllShortcutsKey();
end;

procedure setModule();
begin
  if szone=curModule then exit;
  curModule:=szone;

  for i:=3 to 9 do clearZone(i);
  _mzoneActive[0]:=true;
  _mzoneActive[1]:=true;
  _mzoneActive[2]:=true;
  fillchar(YSCR[56+48],600,0);
  HPOSP[2]:=0;
  setIcon(szone);
  clearStatus();
  clearWorkarea();
  _mzoneActive[0]:=(szone<>0);
  _mzoneActive[1]:=(szone<>1);
  _mzoneActive[2]:=(szone<>2);
  case szone of
    0:begin loadModule('D:DISK.LIB'); showDiskDirectory(); end;
    1:begin loadModule('D:PATHED.LIB'); showPathEditor(); end;
    2:begin loadModule('D:SCENED.LIB'); showScenarioEditor(); end;
   -1:begin loadModule('D:ABOUT.LIB'); showAbout(); end;
  end;
End;

procedure initModules();
begin
  moduleInitialized:=0;
  setZone(0,false,28,49,3,15,@setModule);
  setZone(1,false,32,49,3,15,@setModule);
  setZone(2,false,36,49,3,15,@setModule);
  szone:=-1; setModule();
  for i:=56+48+16 to 56+48+18 do fillchar(Pointer(word(YSCR[i])+8),32,%01010101);
  putTextC(0,63,'00:00.00');
End;