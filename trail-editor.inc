// const

// var
procedure updatePathSection(); Forward;
{$I 'trail-editor-actions.inc'}
{$I 'trail-editor-controls.inc'}

procedure showTrailEditor();
begin
  clearWorkarea();
  clearStatus();
  setControls(%001111); // only Prev/Play/Stop/Next controls
  setControl(-1);
  for i:=56+48+16 to 56+48+18 do fillchar(Pointer(word(YSCR[i])+8),32,%01010101);
  putTextC(0,63,'00:00.00');

  SetScreenWidth(20);
  putImage(_ITRLED,0,0,3,48);

   addZone(0,0,3,12,@doBack2Selector);  // back to trail selector
   addZoneH(@nullProc);               // Options
   addZone(0,12,6,12,@doSwitchPathSection);       // Begin trail
   addZoneV(@doSwitchPathSection);               // Path
   addZoneV(@doSwitchPathSection);               // Finish trail

  for i:=56 to 56+47 do fillchar(Pointer(word(YSCR[i])+10),10,$FF);
End;
