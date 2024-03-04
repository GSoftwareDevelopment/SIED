{$I 'utils.var.inc'}

procedure reduceFileName(var inFN; outFN:PString); Register; Assembler; Asm icl 'asm/reduce_filename.a65' End;
function keyscan2asc(keyscan:Byte):Byte; Assembler; Asm icl 'asm/keyscan2asc.a65' End;
procedure convASC2INT(var s:PString); Register; Assembler; Asm icl 'asm/asc2int.a65' End;

function findText(sPhrase,sText:PString):Byte;
var
  p:Byte;
  t:Byte;
  pLen,tLen:byte;

begin
  pLen:=byte(sPhrase[0]);
  tLen:=byte(sText[0]);
  p:=1; t:=1;
  while (t<=tLen) do
  begin
    if sText[t]=sPhrase[p] then
    begin
      if p=1 then result:=t;
      inc(p); if (p>pLen) then break;
    end
    else
    begin
      if p>1 then begin p:=1; continue; end;
    end;
    inc(t);
  end;
  if p<=pLen then result:=0;
end;
