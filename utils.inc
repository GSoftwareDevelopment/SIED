procedure getLn(chn:Byte; buf:PString); Register; Assembler; Asm icl 'asm/get_line.a65' End;
procedure reduceFileName(var inFN; outFN:PString); Register; Assembler; Asm icl 'asm/reduce_filename.a65' End;

var
  _asc2int:array[0..127] of Byte absolute ASC2INT_ADDR;

function keyscan2asc(keyscan:Byte):Byte; Assembler; Asm icl 'asm/keyscan2asc.a65' End;
procedure convASC2INT(var s:PString); Register; assembler;
Asm
  stx _xreg
  ldy #0
  lda (s),y
  tay
lp:
  lda (s),y
  tax
  lda adr._asc2int,x
  sta (s),y
  dey
  bne lp
  ldx _xreg
End;
