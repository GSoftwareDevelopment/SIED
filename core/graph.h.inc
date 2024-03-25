{$i 'core/graph.var.inc'}

procedure putImage(adr:Pointer; x,y,width,height:Byte); Register; external 'core';
procedure putSprite(adr:Pointer; x,y,width,height:Byte); Register; external 'core';
procedure putSpriteXOR(adr:Pointer; x,y,width,height:Byte); Register; external 'core';

procedure putChar(x,y:Shortint; ch:Char); Register; external 'core';
procedure putCharC(x,y:Shortint; ch:Char); Register; external 'core';

procedure putText(x,y:Shortint; s:PString); Register; overload; external 'core';
procedure putText(adr:Pointer; s:PString); Register; overload; external 'core';

procedure putTextC(x,y:Shortint; s:PString); Register; overload; external 'core';
procedure putTextC(adr:Pointer; s:PString); Register; overload; external 'core';

procedure invert(x,y,w,h:Shortint); Register; external 'core';
procedure blank(x,y,w,h:Shortint); Register; external 'core';

procedure SetScreenWidth(nSW:Byte); Register; external 'core';
procedure wait(f:Byte); Register; external 'core';