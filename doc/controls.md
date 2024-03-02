[Back to CORE](core.md)
# CONTROLS Library

## Constants & Varialbles

```
const
  YCONTROLS         = 55;
```

## Methods

### invertZone(i:Shortint);

### clearWorkarea();

### clearStatus();

### setStatus(s:PString);

### setIcon(n:Shortint);

### setControls(s:Byte);

### setControl(n:Shortint);

### addButton(x,y:Byte; cap:PString; prc:TZoneProc):Shortint;

### addInput(x,y,w:Byte; value:PString; prc:TZoneProc):Shortint;

### doInput(value:PString):Shortint;

