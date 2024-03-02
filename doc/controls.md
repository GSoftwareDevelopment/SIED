[Back to CORE](core.md)
# CONTROLS Library

## Constants & Varialbles

```
const
  YCONTROLS         = 55;
```

## Methods

### invertZone(i:Shortint);

Dokonuje inwersji kolorów wybranej `i` strefy.
Nie są sprawdzane ustawienia strefy!

### clearWorkarea();

Czyści obszar roboczy programu oraz usuwa strefy użytkownika.

### clearStatus();

Czyści linię statusu.

### setStatus(s:PString);

Ustawia tekst w linii statusu.

### setIcon(n:Shortint);

Pozwala wybrać "zakładkę" `n` - tylko efekt, bez wywoływania zdarzenia - w panelu kontrolnym.
Wartość `n` musi być w zakresie 0…2. Inne wartości nie dają efektu.
Jeżeli wybrana wartość `n` była wcześniej wybrana, efektu również nie będzie - podobnie będzie z aktywnością strefy "zakładki"

### setControls(s:Byte);

Pozwala wybrać, które z kontrolek (PREV/PLAY/STOP/NEXT/INS/DEL) będą dostępne na panelu kontrolnym.
Jako parametr, podawana jest wartość typu `byte`, której poszczególne bity oznaczają:
bit 0 - kontrolka Prevoius
bit 1 - kontrolka Play
bit 2 - kontrolka Stop
bit 3 - kontrolka Next
bit 4 - kontrolka Insert
bit 5 - kontrolka Delete

### setControl(n:Shortint);

Pozwala wybrać (efekt wciśnięcia) kontrolkę `n` na panelu kontrolnym.
Wartość `n` musi być w zakresie 0…6, inne wartości nie dadzą żadnego rezultatu.

### addButton(x,y:Byte; cap:PString; prc:TZoneProc):Shortint;

Tworzy przycisk w pozycji `x,y` o treści `cap` oraz przypisuje zdarzenie `prc` (wskaźnik do procedury) które zostanie wykonane, gdy element zostanie naciśnięty.
Pozycja `x` jest ograniczona do znaku w obszarze roboczym, czyli położenie od 0 do 39.
Pozycja `y` to linia w obszarze roboczym programu.

Zwraca numer strefy, jaki został przypisany dla elementu, lub -1, gdy nie było możliwości jego przypisania.

### addInput(x,y,w:Byte; value:PString; prc:TZoneProc):Shortint;

Tworzy pole tekstowe. Podobnie jak w przypadku `addButton` w pozycji `x,y`, lecz o zadanej szerokości `w` (w znakach). Wartość pola wstępnie określa zmienna `value`.
Parametr `prc` to wskaźnik do procedury, która będzie wywołana, gdy element zostanie naciśnięty.
Aktywne pole elementu jest o dwa znaki szersze niż wartość szerokości i tak efektywna strefa elementu to x-1 do x+w+1.

Zwraca numer strefy, jaki został przypisany dla elementu, lub -1, gdy nie było możliwości jego przypisania.

### doInput(value:PString):Shortint;

Akcja edycji pola tekstowego.
Musi być wywołana z procedury przekazanej przez `addInput` jako zdarzenie `prc`.
Zwraca następujące wartości:
-2, gdy pole zostało opuszczone przez kliknięcie w inną strefę
-1, gdy został naciśnięty klawisz ESC
0..31, a dokładniej numer strefy, jaki został przypisany elementowi, gdy został naciśnięty RETURN
