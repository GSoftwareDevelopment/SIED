var
  adr:Pointer;
  p:PByte;

function getFirstFreeTrailId():Shortint;
begin
  result:=0; adr:=pointer($a000);
  while (result<MAXPATHDEFINITIONS) do
  begin
    if pathListPtr[result]<adr then begin dec(result); break; end;
    adr:=pathListPtr[result];
    inc(result);
  end;
  if (result=MAXPATHDEFINITIONS) then result:=-1;
end;

function getTrailName(n:Byte):Pointer;
begin
  // find 'i' entry in table of trail names
  p:=pointer(PATHNAMES_ADDR);
  while (n<>0) do
  begin
    if p^=0 then exit(nil);
    dec(n);
    inc(p,p^+1);
  end;
  result:=p;
end;

function getTrailSize(n:Byte):word;
begin
  result:=word(pathListPtr[n+1])-word(pathListPtr[n]);
end;
