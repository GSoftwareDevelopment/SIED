{$I 'timers.var.inc'}

procedure nullTimer(); Assembler; asm end;

procedure clearTimer(n:Byte);
begin
  _iTimerActive[n]:=false;
  _iTimer[n]:=0;
  _iTimerCall[n]:=@nullTimer;
end;

procedure initTimers();
begin
  for i:=0 to MAX_TIMERS do
    clearTimer(i);
end;

procedure setTimerN(n:Byte; t:Byte; call:TTimerProc);
begin
  _iTimerActive[n]:=true;
  _iTimer[n]:=t;
  _iTimerCall[n]:=call;
end;

function setTimer(t:Smallint; call:TTimerProc):shortint;
begin
  result:=-1;
  for i:=0 to MAX_TIMERS do
    if not _iTimerActive[i] then
    begin
      result:=i;
      setTimerN(i,t,call);
      break;
    end;
end;

procedure checkTimers();
var
  ttm:byte;

begin
  if tm=ttm then exit;

  ttm:=tm;
  for i:=0 to MAX_TIMERS do
  begin
    if _iTimerActive[i] then
    begin
      if _iTimer[i]>0 then
        dec(_iTimer[i])
      else
      begin
        asm
          txa:pha:tya:pha
          lda #false
          sta adr._iTimerActive,y

          tya
          asl @
          tay
          lda adr._iTimerCall,y
          sta jaddr
          lda adr._iTimerCall+1,y
          sta jaddr+1

					lda #<MAIN.nullTimer
          sta adr._iTimerCall,y
          lda #>MAIN.nullTimer
          sta adr._iTimerCall+1,y

          pla:tay:pla:tax

          jsr jaddr:$ffff
        end;
      end;
    end;
  end;
end;
