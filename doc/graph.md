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

Wyświetla obraz podany jako `adr` w pozycji `x` (bajt) i `y` (linia). Szerokość i wysokość obrazu definiują odpowiednio `width` oraz `height`.

### `putSprite(adr:Pointer; x,y:shortint; width,height:shortint);`

Wyświetla sprite podany jako `adr` w pozycji `x` (piksel) i `y` (linia).
Poza dokładnością pozycjonowania, różnica jest taka, że sprite jest nakładany na obszar operacją OR.

### `putSpriteXOR(adr:Pointer; x,y,width,height:Byte);`

Wyświetla sprite podany jako `adr` w pozycji `x` (piksel) i `y` (linia).
Sprite jest nakładany na obszar operacją XOR.

### `putText(x,y:Shortint; s:PString); overload;`

Pozwala wyświetlić tekst `s` we wskazanej pozycji `x` (znak) i `y` (linia).
Ilość znaków w wierszu to 40. Nie istnieje zmiana wiersza, a tekst który wyjdzie poza ekran, zostanie zawinięty i obniżony o jedną linię.
Akceptowane znaki są z przedziału #32…#127 (nie występują małe litery!)
Wyświetlany tekst jest monochromatyczny i jest nakładany na obszar operacją OR.

### `putText(adr:pointer; s:PString); overload;`

Tworzy tekst na podstawie podanego `s` ciągu znaków, rysując w podanym adresie `adr`.
Bitmapa ma szerokość 20 bajtów, zaś wysokość 5 linii.
Akceptowane znaki są z przedziału #32…#127 (nie występują małe litery!)
Rysowany tekst jest monochromatyczny i jest nakładany na obszar operacją OR.

### `putTextC(x,y:Shortint; s:PString);`

Podobnie jak `putText` z tą różnicą, że wyświetlany tekst jest kolorowy (4bpp) i jest nakładany na obszar operacją OR. Nie ma możliwości ustalenia koloru! (jeszcze?)

### `putTextC(adr:pointer; s:PString); overload;`

Analogicznie do `putText(adr:pointer; s:PString); overload;`  z tym, że bitmapa ma szerokość 40 bajtów, a rysowany tekst jest kolorowy.

### `invert(x,y,w,h:Shortint);`

Wykonuje operację odwracania bitów w wyznaczonym obszarze o początku `x` i `y` oraz wymiarach `w` (znak) `h` (linia)

### `blank(x,y,w,h:Shortint);`

Czyści wyznaczony obszar o początku `x` i `y` oraz wymiarach `w` (znak) `h` (linia)

### `SetScreenWidth(nSW:Byte);`

Ustawia szerokość linii przy operacjach rysowania.
Ma wpływ na wszystkie powyższe procedury.
Standardowe wartości `nSW` to:
- 20, dla trybu monochromatycznego (1bpp)
- 40, dla trybu kolorowego (4bpp)
- 16, dla okna gry (monochromatyczne 1bpp)

### `wait(f:Byte);`

Pauza o długości `f` ramek (1/50 dla PAL, 1/60 dla NTSC)

