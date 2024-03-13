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
  _mZoneActive:Array[0..MAX_ZONES] of Boolean absolute $500;
  _mZoneX1:array[0..MAX_ZONES] of Byte absolute $520;
  _mZoneY1:array[0..MAX_ZONES] of Byte absolute $540;
  _mZoneX2:array[0..MAX_ZONES] of Byte absolute $560;
  _mZoneY2:array[0..MAX_ZONES] of Byte absolute $580;
  _mZoneCall:Array[0..MAX_ZONES] of TZoneProc absolute $5a0;

var
  szone:Shortint absolute $50;
  ozone:Shortint absolute $51;
```

## Methods

### nullProc();

Pusta procedura dla zdarzeń.

### setZone(n:Shortint; act:Boolean; x,y,w,h:Byte; prc:TZoneProc);

Ustawia strefę `n` na zadane parametry:
`act` - aktywność strefy;
`x` - pozycja początkowa pozioma (w znakach, dokładność co 4 piksele)
`y` - pozycja początkowa pionowa (w liniach)
`w` - szerokość (w znakach)
`h` - wysokość (w liniach)
`prc` - procedura zdarzenia naciśnięcia strefy

### clearZone(n:Byte);

Kasuje podaną `n` strefę.

### clearWorkZones();

Kasuje strefy użytkownika, tj. od strefy od numeru 9…31

### initInterface();

Inicjacja interfejsu.

### getFreeZone:Shortint;

Zwraca pierwszą wolną strefę użytkownika.

Zwraca numer strefy lub -1, gdy nie została ona znaleziona.

### addZoneN(n:Shortint; x,y,w,h:Byte; prc:TZoneProc):Shortint;

Podobne w działaniu do `setZone`, jednak dla parametru `n` równego -1, dokonuje wyszukania wolnej strefy użytkownika. W przypadku jej znalezienia, przydziela zgodnie z podanymi parametrami.
Dla parametru `n` większego od -1, działa identycznie z `setZone`.

Zwraca numer strefy lub -1, gdy nie została ona przydzielona.

### addZone(x,y,w,h:Byte; prc:TZoneProc):Shortint;

Jak dla `addZoneN` tyle, że nie trzeba podawać parametru `n`.

Zwraca numer strefy lub -1, gdy nie została ona przydzielona.

### addZoneH(prc:TZoneProc):Shortint;

Pozwala utworzyć nową strefę użytkownika, ustawiając pozycję `x` nowej strefy zaraz za (po prawej stronie) ostatnio utworzoną.

### addZoneHN(n:Shortint; prc:TZoneProc):Shortint;

Podobnie jak `addZoneH` z możliwością ustalenia numeru strefy.

### addZoneV(prc:TZoneProc):Shortint;

Pozwala utworzyć nową strefę użytkownika, ustawiając pozycję `y` nowej strefy zaraz pod, ostatnio otworzoną.

### addZoneVN(n:Shortint; prc:TZoneProc):Shortint;

Podobnie jak `addZoneV` z możlwioścą ustalenia numeru strefy.

### checkZones:Boolean;

Funkcja poolingu dla kursora i stref.
Zwraca `true`, jeśli strefa została naciśnięta.

W zmiennej `szone`, po detekcji naciśnięcia, będzie znajdował się numer naciśniętej strefy, do momentu następnego wywołania funkcji.

Obsługuje graficznie strefy panelu kontrolnego!

### procedure callZoneProc();

Wywołanie przypisanej procedury dla strefy.
W zmiennej `szone` musi być umieszczony numer strefy

### procedure runInterface();

Wykonuje podstawowe operacje związane z pollingiem interfejsu.
Nieskończona pętla interfejsu.
