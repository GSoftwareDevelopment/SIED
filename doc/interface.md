[Back to CORE](core.md)
# INTERFACE Library

## Constants & Varialbles

```
const
  MAX_ZONES         = 31;
  USER_ZONES_BEGIN  = 9;

type
  TZoneProc = procedure();

var
  _mzoneActive:Array[0..MAX_ZONES] of Boolean absolute $500;
  _mzoneX1:array[0..MAX_ZONES] of Byte absolute $520;
  _mzoneY1:array[0..MAX_ZONES] of Byte absolute $540;
  _mzoneX2:array[0..MAX_ZONES] of Byte absolute $560;
  _mzoneY2:array[0..MAX_ZONES] of Byte absolute $580;
  _mzonePROC:Array[0..MAX_ZONES] of TZoneProc absolute $5a0;

var
  szone:Shortint absolute $50;
  ozone:Shortint absolute $51;
```

## Methods

### nullProc();

### setZone(n:Shortint; act:Boolean; x,y,w,h:Byte; prc:TZoneProc);

### clearZone(n:Byte);

### clearWorkZones();

### initInterface();

### getFreeZone:Shortint;

### addZoneN(n:Shortint; x,y,w,h:Byte; prc:TZoneProc):Shortint;

### addZone(x,y,w,h:Byte; prc:TZoneProc):Shortint;

### addZoneH(prc:TZoneProc):Shortint;

### addZoneHN(n:Shortint; prc:TZoneProc):Shortint;

### addZoneV(prc:TZoneProc):Shortint;

### addZoneVN(n:Shortint; prc:TZoneProc):Shortint;

### checkZones:Boolean;
