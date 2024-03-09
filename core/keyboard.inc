procedure __noShortcurKey();
begin
  keyb:=$FF;
end;

procedure clearAllShortcutsKey();
begin
  for i:=0 to 63 do
    __SCKeyVec[i]:=@__noShortcurKey;
end;

procedure clearShortcutKey(key:byte); register;
begin
  key:=key and $3F;
  __SCKeyVec[key]:=@__noShortcurKey;
end;

procedure initShortcutKeyboard();
begin
  clearAllShortcutsKey();
  allowShortcutKeys:=true;
  _callShortcutKeys:=$FF;
end;

procedure addShortcutKey(key:byte; prc:TKeyProc); register;
begin
  key:=key and $3F;
  __SCKeyVec[key]:=prc;
end;

function checkShortcutKeyboard(mKey:Byte):Boolean; Assembler;
asm
  lda _callShortcutKeys
  cmp #$FF
  bne setTrue

  lda #$FF
  sta _callShortcutKeys
  lda #false
  sta Result

  lda allowShortcutKeys
  beq @exit

  lda KEYB
  cmp #$FF
  beq @exit

  lda mKey
  beq skipMod

  lda KEYB
  and mKey
  beq @exit

skipMod:
  lda KEYB
  and #$3F
  clc
  asl @
  tay

  lda adr.__SCKeyVec,y
  cmp #<__noShortcurKey
  bne setTrue
  lda adr.__SCKeyVec+1,y
  cmp #>__noShortcurKey
  beq @exit

setTrue:
  lda KEYB
  sta _callShortcutKeys
  lda #true
  sta result
end;

procedure callShortcutKeyboard(); assembler;
asm
  lda _callShortcutKeys
  and #%00111111   ; remove mod-keys (Control, Shift)
  asl @
  tay

  lda adr.__SCKeyVec,y
  sta keyJump
  lda adr.__SCKeyVec+1,y
  sta keyJump+1

  lda #$FF
  sta _callShortcutKeys

  jsr keyJump:$FFFF

  lda _callShortcutKeys
  cmp #$FF
  bne @+

  lda #$FF
  sta KEYB
@:
end;
