{$I 'core/utils.var.inc'}

procedure reduceFileName(var inFN; outFN:PString); Register; Assembler; external 'core';
function keyscan2asc(keyscan:Byte):Byte; Assembler; external 'core';
procedure convASC2INT(var s:PString); Register; Assembler; external 'core';
