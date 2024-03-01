{$define BASICOFF}
{$define ROMOFF}
{$define NOROMFONT}
{$LIBRARYPATH 'core/bin/'}

library pathed:$4000;

const
{$I 'data/data-mem.inc'}

{$I 'core/keyboard.var.inc'}
// {$I 'core/pmg.var.inc'}

// {$I 'core/assets.h.inc'}
// {$I 'core/cursor.h.inc'}
{$I 'core/graph.h.inc'}
{$I 'core/interface.h.inc'}
{$I 'core/controls.h.inc'}

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

exports
  showScenarioEditor;

end.