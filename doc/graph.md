[Back to CORE](core.md)
# GRAPH Library

## Constants & Varialbles

```
var
  DLIV:Pointer absolute $200;
  [volatile] NMIEN:Byte absolute $D40E;
  // SDMACTL:Byte absolute $22F;
  SDLST:Pointer absolute $230;
  GTIACS:Byte absolute $026F;
  PFCOL0:Byte absolute 708;
  PFCOL1:Byte absolute 709;
  PFCOL2:Byte absolute 710;
  PFCOL3:Byte absolute 711;
  PFCOL4:Byte absolute 712;
  activePage:Byte absolute $d4;
  scr:Pointer absolute 88;
  YSCR:Array[0..127] of Pointer absolute SCREEN_LINES_ADDR;
  AFONT:Array[0..127] of Word absolute FONTS_ADDR;
  AFONTC:Array[0..63] of Word absolute FONTS_ADDR+$100;
```

## Methods

### `putImage(adr:Pointer; x,y,width,height:Byte);`

### `putSprite(adr:Pointer; x,y:shortint; width,height:shortint);`

### `putSpriteXOR(adr:Pointer; x,y,width,height:Byte);`

### `putText(x,y:Shortint; s:PString);`

### `putTextC(x,y:Shortint; s:PString);`

### `invert(x,y,w,h:Shortint);`

### `lank(x,y,w,h:Shortint);`

### `SetScreenWidth(nSW:Byte);`

### `clearPage(page:Byte);`

### `wait(f:Byte);`
