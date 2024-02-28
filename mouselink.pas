unit MouseLink;

interface
var
  isIntelliMouse:Boolean = false;

procedure initMouse;
function  resetMouse:Boolean;

implementation
var
  volatile PORTA:Byte absolute $D300;
  volatile PACTL:Byte absolute $D302;
  tm:Byte absolute $14;
  ntm:Byte;

const
  _reset  = %0001 shl 4;
  _clock  = $0010 shl 4;
  _latch  = %0100 shl 4;
  _data   = %1000 shl 4;

function resetMouse:Boolean;
begin
  ntm:=Byte(tm+17); // 17=340ms
  PORTA:=PORTA and (NOT _reset); // reset MouseLink
  PORTA:=PORTA or _reset;
  while ((PORTA and _data)<>0) or (tm<>ntm) do; // wait for DATA LOW or Timeout
  // After a reset, the DATA pin is set to LOW.
  result:=(PORTA and _data)=0;

  // -- detect Intelimouse signal
  // IntelliMouse detection is indicated by a
  // low state on DATA for 100ms, after which a
  // high state is set on DATA for 100ms.
  ntm:=Byte(tm+5); // 5=100ms
  while (tm<>ntm) do // wait
  if (PORTA and _data)=1 then
  begin
    ntm:=Byte(tm+5); // 5=100ms
    while ((PORTA and _data)<>0) or (tm<>ntm) do // wait for DATA LOW or Timeout
    if (PORTA and _data)=0 then isIntelliMouse:=true;
  End;
End;

procedure initMouse;
begin
  isIntelliMouse:=false;
  PACTL:=PACTL and %11111011;  // set port A as flow control
  PORTA:=%01110000;       // set JOY#1 as MouseLink interface
  PACTL:=PACTL or  %00000100;  // set port A as data Register
  PORTA:=_reset and _clock and _latch;
  resetMouse;
End;

procedure getMouse;
var
  dta,bit,b,i:Byte;

begin
  PORTA:=PORTA or _latch;
  ntm:=Byte(tm+5); // 5=100ms
  while ((PORTA and _data)<>0) or (tm<>ntm) do // wait for DATA LOW or Timeout
  while ((PORTA and _data)=0) or (tm<>ntm) do // wait for DATA HIGH or Timeout

  for i:=0 to 3 do
  begin
    dta:=0;
    for b:=7 downto 0 do
    begin
      PORTA:=PORTA and (NOT _clock);
      bit:=PORTA and _data;
      dta:=dta or bit;
      PORTA:=PORTA or _clock;
      dta:=dta shl 1;
    End;
  End;
  PORTA:=PORTA and (NOT _latch)
End;

End.