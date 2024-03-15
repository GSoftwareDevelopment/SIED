const
  MAX_ZONES         = 31;
  USER_ZONES_BEGIN  = 9;

type
  TZoneProc = procedure();

var
  _mZoneActive:Array[0..MAX_ZONES] of Boolean     absolute IZONE_ADDR+$00;
  _mZoneX1:array[0..MAX_ZONES] of Byte            absolute IZONE_ADDR+$20;
  _mZoneY1:array[0..MAX_ZONES] of Byte            absolute IZONE_ADDR+$40;
  _mZoneX2:array[0..MAX_ZONES] of Byte            absolute IZONE_ADDR+$60;
  _mZoneY2:array[0..MAX_ZONES] of Byte            absolute IZONE_ADDR+$80;
  _mZoneCall:Array[0..MAX_ZONES] of TZoneProc     absolute IZONE_ADDR+$A0;
  _mZoneInCall:Array[0..MAX_ZONES] of TZoneProc   absolute IZONE_ADDR+$E0;

var
  __invertZone:Boolean absolute $41;
  szone:Shortint absolute $50;
  ozone:Shortint absolute $51;
  nzone:Shortint absolute $57;
