const
  MAX_ZONES         = 31;
  USER_ZONES_BEGIN  = 9;

type
  TZoneProc = procedure();

var
  zoneActive:Array[0..MAX_ZONES] of Boolean       absolute IZONE_ADDR+$00;
  _mZoneX1:array[0..MAX_ZONES] of Byte            absolute IZONE_ADDR+$20;
  _mZoneY1:array[0..MAX_ZONES] of Byte            absolute IZONE_ADDR+$40;
  _mZoneX2:array[0..MAX_ZONES] of Byte            absolute IZONE_ADDR+$60;
  _mZoneY2:array[0..MAX_ZONES] of Byte            absolute IZONE_ADDR+$80;
  zoneCall:Array[0..MAX_ZONES] of TZoneProc       absolute IZONE_ADDR+$A0;
  zoneInOutCall:Array[0..MAX_ZONES] of TZoneProc  absolute IZONE_ADDR+$E0;
  zoneHint:Array[0..MAX_ZONES] of PString         absolute IZONE_ADDR+$120;
  zoneTabIndex:Array[0..MAX_ZONES] of Shortint         absolute IZONE_ADDR+$160;

var
  selZone:Shortint absolute $6B;
  oldZone:Shortint absolute $6C;
  nowZone:Shortint absolute $6D;
  curZone:Shortint absolute $6E;
  allowInverZone:Boolean absolute $6F;
  hintZone:shortint absolute $70;
  tabZone:shortint absolute $71;
  tabIndex:shortint absolute $72;