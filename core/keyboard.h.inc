{$I 'core/keyboard.var.inc'}

procedure initShortcutKeyboard(); external 'core';
procedure clearAllShortcutsKey(); external 'core';
procedure clearShortcutKey(key:byte); register; external 'core';
procedure addShortcutKey(key:byte; prc:TKeyProc); register; external 'core';
procedure checkShortcutKeyboard(); assembler; external 'core';