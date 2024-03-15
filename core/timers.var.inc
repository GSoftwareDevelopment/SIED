const
  MAX_TIMERS        = 7;

type
  TTimerProc = procedure();

var
  _iTimerActive:Array[0..MAX_TIMERS] of Boolean   absolute ITIMER_ADDR+$00;
  _iTimer:Array[0..MAX_TIMERS] of Byte            absolute ITIMER_ADDR+$08;
  _iTimerCall:Array[0..MAX_TIMERS] of TTimerProc  absolute ITIMER_ADDR+$10;
