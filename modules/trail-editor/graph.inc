procedure point(x,y:Shortint; c:Byte);
var
  bit:byte;

begin
  if (y<0) or (y>47) then exit;
  bit:=x and 7; x:=(8+x) shr 3;
  scr:=YSCR[4+y]+x;
  bit:=PBIT[bit];
  if c=0 then
    poke(scr,peek(scr) and not bit)
  else
    poke(scr,peek(scr) or bit);
end;

procedure TLPoint(c:Byte);
var
  m,b:Byte;
  pos:Smallint;
  bit:Byte;

begin
  pos:=TLPosition-TLShift;
  if (pos>=0) and (pos<TIMELINELENGTH) then
  begin
    TLPtr:=Pointer(TLBUF_ADDR+TIMELINE_SIZEB+pos shr 2);
    bit:=pos and 3;
    m:=P4MASK[bit]; if c>0 then b:=P4COLOR[c-1][bit] else b:=0;
    poke(TLPtr,(peek(TLPtr) and m) or b); inc(TLPtr,TIMELINE_SIZEB);
    poke(TLPtr,(peek(TLPtr) and m) or b); inc(TLPtr,TIMELINE_SIZEB);
    poke(TLPtr,(peek(TLPtr) and m) or b);
  end;
end;
