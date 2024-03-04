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
  AllowShortcutKeys:=true;
end;

procedure addShortcutKey(key:byte; prc:TKeyProc); register;
begin
  key:=key and $3F;
  __SCKeyVec[key]:=prc;
end;

procedure checkShortcutKeyboard(); assembler;
asm
  lda allowShortcutKeys
  beq xitSK

  lda KEYB
  cmp #$FF
  beq xitSK

  and #%00111111   ; remove mod-keys (Control, Shift)
  asl @
  tay

  lda adr.__SCKeyVec,y
  sta keyJump
  lda adr.__SCKeyVec+1,y
  sta keyJump+1

  // lda keyJump
  // cmp #<main.__noShortcurKey
  // bne jump
  // lda keyJump+1
  // cmp #>main.__noShortcurKey
  // beq xitsk

jump:
  lda KEYB
  and #%11000000
  jsr keyJump:main.__noShortcurKey

  lda #$FF
  sta KEYB

xitSK:
end;