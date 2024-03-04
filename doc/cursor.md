[Back to CORE](core.md)
# CURSOR Library

## Constants & Varialbles

```
const
  MBUT_NONE   = 0;
  MBUT_LEFT   = 1;

var
    mRefresh:Boolean  absolute $5c;         toggle cursor position refresh
         _mx:Byte     absolute $52;         cursor X position in pixels
         _my:Byte     absolute $53;         cursor Y position in pixels
        _omy:Byte     absolute $63;         cursor Y old position in pixels
          mx:Shortint absolute $55;         cursor X position in characters
          my:Shortint absolute $54;         cursor Y position in Characters
         mdx:Shortint absolute $5b;
         mdy:Shortint absolute $5a;
       stick:Byte     absolute $278;
     mbutton:Byte     absolute $4a;         button state
    ombutton:Byte     absolute $4b;

  CURSORSPR :Pointer  absolute $5e;         pointer to cursor shape
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

### procedure setCursorPos(x,y:Byte); Register;

Ustawia pozycję kursora w podanym miejscu `x` i `y`.
Pozycja uwzględnia lewy górny róg ekranu i wyrażana jest w pikselach.