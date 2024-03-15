{$i 'core/timers.var.inc'}

procedure clearTimer(n:Byte); external 'core';
procedure initTimers(); external 'core';
procedure setTimerN(n:Byte; t:Smallint; call:TTimerProc); external 'core';
function setTimer(t:Smallint; call:TTimerProc):shortint; external 'core';
procedure checkTimers(); external 'core';
