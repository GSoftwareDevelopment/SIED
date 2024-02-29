{$i 'core/graph.var.inc'}

procedure putImage(adr:Pointer; x,y,width,height:Byte); Register; external 'core';
procedure putSprite(adr:Pointer; x,y:shortint; width,height:shortint); Register; external 'core';
procedure putSpriteXOR(adr:Pointer; x,y,width,height:Byte); Register; external 'core';
procedure putText(x,y:Shortint; s:PString); Register; external 'core';
procedure putTextC(x,y:Shortint; s:PString); Register; external 'core';
procedure invert(x,y,w,h:Shortint); Register; external 'core';
procedure blank(x,y,w,h:Shortint); Register; external 'core';

procedure SetScreenWidth(nSW:Byte); Register; external 'core';
procedure clearPage(page:Byte); Register; external 'core';
procedure wait(f:Byte); Register; external 'core';