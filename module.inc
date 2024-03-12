var
  curModule:Byte = -128;
  moduleInitialized:Byte absolute $62;

procedure loadModule(FN:PString);
begin
  setCursorShapeAnchor(3,3); setCursorShape(_WAIT);
  setStatus('LOADING MODULE...');
  XIO($28, 2, 4, $00, FN);
  if IOResult<>1 then setStatus('ERROR LOADING MODULE!');
  setCursorShapeAnchor(0,0); setCursorShape(_ARROW);
end;

procedure setModule(); forward;

procedure toZone(n:byte);
begin
  szone:=n; setCursorInZone(n);
end;

procedure keyDisk(); Keep;
begin
  if _modKey<>(KMOD_CTRL+KMOD_SHIFT) then exit;
  toZone(0); setModule();
end;

procedure keyTrailEd();Keep;
begin
  if _modKey<>(KMOD_CTRL+KMOD_SHIFT) then exit;
  toZone(1); setModule();
end;

procedure keyScenarioEd();Keep;
begin
  if _modKey<>(KMOD_CTRL+KMOD_SHIFT) then exit;
  toZone(2); setModule();
end;

procedure shortKeys(); Assembler;
asm
  dta k_1,a(MAIN.keyDisk)
  dta k_2,a(MAIN.keyTrailEd)
  dta k_3,a(MAIN.keyScenarioEd)
end;

procedure setModule();
begin
  if szone=curModule then exit;
  curModule:=szone;

  HPOSM[2]:=0; HPOSM[3]:=0;
  _mzoneActive[0]:=true;
  _mzoneActive[1]:=true;
  _mzoneActive[2]:=true;
  fillchar(YSCR[56+48],800,0);
  HPOSP[2]:=0;
  setIcon(szone);
  clearStatus();
  clearWorkarea();
  setControls(0);
  _mzoneActive[0]:=(szone<>0);
  _mzoneActive[1]:=(szone<>1);
  _mzoneActive[2]:=(szone<>2);
  case szone of
    0: loadModule('D:DISK.LIB');
    1: loadModule('D:TRAIL.LIB');
    2: loadModule('D:SCENED.LIB');
   -1: loadModule('D:ABOUT.LIB');
  end;
  registerShortcutKeys(@shortKeys,3);
End;

procedure initModules();
begin
  moduleInitialized:=0;
  setZone(0,false,28,49,3,15,@setModule);
  setZone(1,false,32,49,3,15,@setModule);
  setZone(2,false,36,49,3,15,@setModule);
   addZoneN(3,1,YCONTROLS,3,7,@nullProc);   // previous
  addZoneHN(4,@nullProc);                   // play
  addZoneHN(5,@nullProc);                   // stop
  addZoneHN(6,@nullProc);                   // next

   addZoneN(7,15,YCONTROLS,3,7,@nullProc);  // insert
  addZoneHN(8,@nullProc);                   // delete

  szone:=-1; setModule();
End;