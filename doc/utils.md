[Back to CORE](core.md)
# UTILS Library

## Constants & Varialbles

```
var
  _asc2int:array[0..127] of Byte absolute ASC2INT_ADDR;
```

## Methods

### reduceFileName(var inFN; outFN:PString);

Pozwala zredukować wskazany ciąg `inFN` nazwy pliku do nie zbędnych dla systemu, znaków - usuwa białe znaki, łączy nazwę z rozszerzeniem dając kropkę pomiędzy.
Wynik funkcji umieszczany jest w `outFN`.

### convASC2INT(var s:PString);

Konwertuje ciąg znaków ATASCII do kodów ekranowych (INTERNAL)
Korzysta przy tym z tablicy konwersji `asc2int`

### keyscan2asc(keyscan:Byte):Byte;

Konwertuje kod skaningowy klawiartry do kodu ATASCI.
Korzysta przy tym z tablicy konwersji, której adres wskazywany jest przez wektor pod adresem $79 (KEYDEFP)

### function findText(sPhrase,sText:PString):Byte;

Wyszukuje podaną frazę `sPhrase` w ciągu `sText`.
Zwraca pozycję występowania frazy, lub 0, gdy niezostała znaleziona.
