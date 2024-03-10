// {$define BASICOFF}
// {$define ROMOFF}
// {$define NOROMFONT}
library core:$DB00;

const
{$I '../data/data-mem.inc'}
{$I '../assets/assets.inc'}
{$I 'keyboard.var.inc'}
{$I 'utils.inc'}

var
  tm:Byte absolute $14;
  i:shortint absolute $3e;
  moduleInitialized:Byte absolute $62;

//
//
//

{$I 'cursor.inc'}
{$I 'graph.inc'}
procedure invertZone(i:Shortint); Forward;
procedure setIcon(n:Shortint); Forward;
procedure setControl(n:Shortint); Forward;

{$I 'keyboard.inc'}
{$I 'interface.inc'}
{$I 'controls.inc'}

exports
// assets
  _CONTROLS,
  _ICONS,
  _ICARD,
  _IDISK,
  _ITRLSEL,
  _ITRLED,
  _VSCROLL,
  _ERASEINPUT,
  _ARROW,
  _WAIT,

// graph
  putImage,
  putSprite,
  putSpriteXOR,
  putText,
  putChar,
  putTextC,
  invert,
  blank,
  SetScreenWidth,
  clearPage,
  wait,

// cursor
  setCursorShape,
  setCursorShapeAnchor,
  initCursor,
  setCursorPos,

// interface
  nullProc,
  setZone,
  clearZone,
  clearWorkZones,
  initInterface,
  getFreeZone,
  addZone,
  addZoneN,
  addZoneH,
  addZoneHN,
  addZoneV,
  addZoneVN,
  setCursorInZone,
  checkZones,
  callZoneProc,
  runInterface,

// controls
  invertZone,
  blankZone,
  clearWorkarea,
  clearStatus,
  setStatus,
  setIcon,
  setControls,
  setControl,
  addButton,
  addInput,
  doInput,

// utils
  _asc2int,
  reduceFileName,
  keyscan2asc,
  convASC2INT,
  findText,

// keyboard
  __SCKeyVec,
  clearAllShortcutsKey,
  clearShortcutKey,
  addShortcutKey,
  initShortcutKeyboard,
  registerShortcutKeys,
  unregisterShortcutKeys,
  checkShortcutKeyboard,
  callShortcutKeyboard;

begin
  moduleInitialized:=0;
  AllowShortcutKeys:=false;
end.