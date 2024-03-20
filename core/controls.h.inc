{$I 'core/controls.var.inc'}

procedure invertZone(i:Shortint); external 'core';
procedure blankZone(i:Shortint); external 'core';
procedure clearWorkarea(); external 'core';
procedure clearStatus(); external 'core';
procedure setStatus(s:PString); external 'core';
procedure doHintZone(); external 'core';
procedure setIcon(n:Shortint); external 'core';
procedure setControls(s:Byte); external 'core';
procedure setControl(n:Shortint); external 'core';
function addButton(x,y:Byte; cap:PString; prc:TZoneProc):Shortint; external 'core';
function addInput(x,y,w:Byte; value:PString; prc:TZoneProc):Shortint; external 'core';
function doInput(var sEDIT:String):Shortint; external 'core';
