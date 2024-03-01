{$define BASICOFF}
{$define ROMOFF}
{$define NOROMFONT}
{$LIBRARYPATH 'core/bin/'}

library pathed:$4000;

const
{$I 'data/data-mem.inc'}

{$I 'core/keyboard.var.inc'}
{$I 'core/pmg.var.inc'}

{$I 'core/assets.h.inc'}
// {$I 'core/cursor.h.inc'}
{$I 'core/graph.h.inc'}
{$I 'core/interface.h.inc'}
{$I 'core/controls.h.inc'}

procedure showPathEditor();
begin
   addZoneN(3,1,YCONTROLS,3,7,@nullProc);  // previous
  addZoneHN(4,@nullProc);  // play
  addZoneHN(5,@nullProc);  // stop
  addZoneHN(6,@nullProc);  // next

   addZoneN(7,15,YCONTROLS,3,7,@nullProc);  // insert
  addZoneHN(8,@nullProc);  // delete

  setControl(-1);
  HPOSP[2]:=44; PCOL[2]:=$E6; SIZEP[2]:=%11;
  SetScreenWidth(20);
  putImage(_IPATH,0,0,3,48);

   addZone(0,0,3,12,@nullProc); // list
  addZoneV(@nullProc); // pen
  addZoneV(@nullProc); // path 1
  addzoneV(@nullProc); // sprite
   addZone(3,0,3,12,@nullProc);  // arrow
  addZoneV(@nullProc); // line
  addZoneV(@nullProc); // path 2
End;

exports
  showPathEditor;

end.