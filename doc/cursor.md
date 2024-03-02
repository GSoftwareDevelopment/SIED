[Back to CORE](core.md)
# CURSOR Library

## Constants & Varialbles

```
const
  MBUT_NONE   = 0;
  MBUT_LEFT   = 1;

var
    mRefresh:Boolean  absolute $5c;
  mAllowKeys:Boolean  absolute $5d;
         _mx:Byte     absolute $52;
         _my:Byte     absolute $53;
          mx:Shortint absolute $55;
          my:Shortint absolute $54;
         mdx:Shortint absolute $5b;
         mdy:Shortint absolute $5a;
       stick:Byte     absolute $278;
     mbutton:Byte     absolute $4a;
    ombutton:Byte     absolute $4b;

  CURSORSPR :Pointer  absolute $5e;
  mpivotX   :shortint absolute $60;
  mpivotY   :shortint absolute $61;
```

## Resources

_ARROW
_WAIT

## Methods

### setCursorShape(spr:Pointer);

Ustawia kształt kursora na podany przez wskaźnik `spr`.
Dane wyglądu kursora opisuje 7 bajtów.

### setCursorShapeAnchor(x,y:Shortint);

Ustawia aktywny punkt kursora.

### initCursor(VBLPtr:Pointer);

Inicjuje obsługę kursora. Parametr `VBLPtr` musi wskazywać na właściwą procedurę obsługi wskaźnika. Jest ona podłączana do przerwania VBlank.

