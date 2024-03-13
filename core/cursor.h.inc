{$i 'core/cursor.var.inc'}

var
  _ARROW:Pointer external 'core';
  _WAIT:Pointer external 'core';
  _CROSS:Pointer external 'core';

procedure setCursorShape(spr:Pointer); Register; assembler; external 'core';
procedure setCursorShapeAnchor(x,y:Shortint); Register; assembler; external 'core';
procedure setCursor(cur:Pointer); Register; Assembler; external 'core';
procedure initCursor(VBLPtr:Pointer); external 'core';
procedure setCursorPos(x,y:Byte); Register; external 'core';