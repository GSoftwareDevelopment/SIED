{$i 'core/cursor.var.inc'}

var
  _ARROW:Pointer external 'core';
  _WAIT:Pointer external 'core';

procedure setCursor(spr:Pointer); Register; assembler; external 'core';
procedure setPivot(x,y:Shortint); Register; assembler; external 'core';
procedure initCursor(VBLPtr:Pointer); external 'core';
