{$I 'utils.var.inc'}

procedure reduceFileName(var inFN; outFN:PString); Register; Assembler; Asm icl 'asm/reduce_filename.a65' End;
function keyscan2asc(keyscan:Byte):Byte; Assembler; Asm icl 'asm/keyscan2asc.a65' End;
procedure convASC2INT(var s:PString); Register; Assembler; Asm icl 'asm/asc2int.a65' End;
