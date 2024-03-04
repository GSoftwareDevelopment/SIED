// {$define BASICOFF}
// {$define ROMOFF}
// {$define NOROMFONT}
library core:$d800;

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

{$I 'interface.inc'}
{$I 'controls.inc'}
{$I 'keyboard.inc'}

exports
// assets
  _CONTROLS,
  _ICONS,
  _ICARD,
  _IDISK,
  _IPATH,
  _ARROW,
  _WAIT,

// graph
  putImage,
  putSprite,
  putSpriteXOR,
  putText,
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
  checkShortcutKeyboard;

begin
  moduleInitialized:=0;
  AllowShortcutKeys:=false;
end.