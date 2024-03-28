var
  curModule:Byte = -128;
  moduleInitialized:Byte absolute $62;

procedure loadModule(FN:PString);
begin
  setCursor(_WAIT);
  setStatus('LOADING MODULE...');
  XIO($28, 2, 4, $00, FN);
  if IOResult<>1 then setStatus('ERROR LOADING MODULE!');
  setCursor(_ARROW);
end;

procedure setModule(); Keep; Forward;
procedure moduleShortKeys(); Assembler;
asm
  dta k_1,  a(MAIN.setModule)
  dta k_2,  a(MAIN.setModule)
  dta k_3,  a(MAIN.setModule)
end;

procedure setModule(); Keep;
begin
  if KEYB<>$FF then
  begin
    case byte(KEYB+_modKey) of
      k_1 + KMOD_CTRL+KMOD_SHIFT: curModule:=0;
      k_2 + KMOD_CTRL+KMOD_SHIFT: curModule:=1;
      k_3 + KMOD_CTRL+KMOD_SHIFT: curModule:=2;
    else
      exit;
    end;
  end
  else
    curModule:=curZone;

  if not zoneActive[curModule] then exit;

  initTimers();

  for i:=0 to 3 do HPOSM[i]:=0;
  HPOSP[3]:=0;
  zoneActive[0]:=true;
  zoneActive[1]:=true;
  zoneActive[2]:=true;
  fillchar(YSCR[56+48],800,0);
  HPOSP[2]:=0;
  setIcon(curModule);
  clearStatus();
  clearWorkarea();
  setControls(0);
  zoneActive[0]:=(curModule<>0);
  zoneActive[1]:=(curModule<>1);
  zoneActive[2]:=(curModule<>2);
  case curModule of
    0: loadModule('D:DISK.LIB');
    1: loadModule('D:TRAIL.LIB');
    2: loadModule('D:SCENED.LIB');
   -1: loadModule('D:ABOUT.LIB');
  end;
  registerShortcutKeys(@moduleShortKeys,3);
End;

procedure initModules();
begin
{$IFDEF PRELOAD_DATA}
  moduleInitialized:=$02;
{$ELSE}
  moduleInitialized:=0;
{$ENDIF}
  setZone(0,false,28,49,3,15,@setModule);
  assignHintToZone(0,'DISK OPERATIONS');
  setZone(1,false,32,49,3,15,@setModule);
  assignHintToZone(1,'TRAIL EDITOR');
  setZone(2,false,36,49,3,15,@setModule);
  assignHintToZone(2,'SCENARIO EDITOR');

  curZone:=-1; setModule();
End;