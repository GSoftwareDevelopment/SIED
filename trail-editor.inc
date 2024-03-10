// const

// var

{$I 'trail-editor-actions.inc'}
{$I 'trail-editor-controls.inc'}

procedure showTrailEditor();
begin
  clearWorkarea();
  setControls(%001111); // only Prev/Play/Stop/Next controls
  setControl(-1);
  SetScreenWidth(20);
  putImage(_ITRLED,0,0,3,48);

   addZone(0,0,3,12,@back2Selector); // back to trail selector
  addZoneV(@nullProc);          // pen
  addZoneV(@nullProc);          // path 1
  addzoneV(@nullProc);          // sprite
   addZone(3,0,3,12,@nullProc); // arrow
  addZoneV(@nullProc);          // line
  addZoneV(@nullProc);          // path 2
End;
