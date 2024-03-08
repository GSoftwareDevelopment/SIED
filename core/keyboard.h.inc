{$I 'core/keyboard.var.inc'}

procedure initShortcutKeyboard(); external 'core';
procedure clearAllShortcutsKey(); external 'core';
procedure clearShortcutKey(key:byte); register; external 'core';
procedure addShortcutKey(key:byte; prc:TKeyProc); register; external 'core';
function checkShortcutKeyboard(mKey:Byte):Boolean; Assembler; external 'core';
procedure callShortcutKeyboard(); external 'core';