{$i 'core/interface.var.inc'}

procedure nullProc(); external 'core';
procedure setZone(n:Shortint; act:Boolean; x,y,w,h:Byte; prc:TZoneProc); external 'core';
procedure clearZone(n:Byte); external 'core';
procedure clearWorkZones(); external 'core';
procedure initInterface(); external 'core';
function getFreeZone:Shortint; external 'core';
function addZoneN(n:Shortint; x,y,w,h:Byte; prc:TZoneProc):Shortint; external 'core';
function addZone(x,y,w,h:Byte; prc:TZoneProc):Shortint; external 'core';
function addZoneH(prc:TZoneProc):Shortint; external 'core';
function addZoneHN(n:Shortint; prc:TZoneProc):Shortint; external 'core';
function addZoneV(prc:TZoneProc):Shortint; external 'core';
function addZoneVN(n:Shortint; prc:TZoneProc):Shortint; external 'core';
function checkZones:Boolean; external 'core';