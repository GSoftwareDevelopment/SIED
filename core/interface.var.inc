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
  _mZoneHint:Array[0..MAX_ZONES] of PString       absolute IZONE_ADDR+$120;

var
  szone:Shortint absolute $6B;
  ozone:Shortint absolute $6C;
  nzone:Shortint absolute $6D;
  czone:Shortint absolute $6E;
  allowInverZone:Boolean absolute $6F;
