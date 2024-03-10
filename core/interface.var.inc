const
  MAX_ZONES         = 31;
  USER_ZONES_BEGIN  = 9;

type
  TZoneProc = procedure();

var
  _mzoneActive:Array[0..MAX_ZONES] of Boolean absolute IZONE_ADDR+$00;
  _mzoneX1:array[0..MAX_ZONES] of Byte        absolute IZONE_ADDR+$20;
  _mzoneY1:array[0..MAX_ZONES] of Byte        absolute IZONE_ADDR+$40;
  _mzoneX2:array[0..MAX_ZONES] of Byte        absolute IZONE_ADDR+$60;
  _mzoneY2:array[0..MAX_ZONES] of Byte        absolute IZONE_ADDR+$80;
  _mzonePROC:Array[0..MAX_ZONES] of TZoneProc absolute IZONE_ADDR+$a0;

var
  szone:Shortint absolute $50;
  ozone:Shortint absolute $51;
