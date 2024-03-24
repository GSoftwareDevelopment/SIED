// {$define BASICOFF}
// {$define ROMOFF}
// {$define NOROMFONT}
library core:$DC00;

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
{$I 'graph.inc'}
{$I 'cursor.inc'}
{$I 'keyboard.inc'}
{$I 'timers.inc'}

procedure invertZone(i:Shortint); Forward;
procedure setIcon(n:Shortint); Forward;
procedure setControl(n:Shortint); Forward;
procedure doHintZone(); Forward; Keep;
{$I 'interface.inc'}
{$I 'controls.inc'}

exports
// assets
  _CONTROLS,
  _ICONS,
  _ICARD,
  _VSCROLL,
  _ERASEINPUT,
  _RADIOBUT,
  _ARROW,
  _WAIT,
  _BEAM,
  _CROSS,

// graph
  putImage,
  putSprite,
  putSpriteXOR,
  putText,
  putChar,
  putTextC,
  putCharC,
  invert,
  blank,
  SetScreenWidth,
  clearPage,
  wait,

// cursor
  setCursorShape,
  setCursorShapeAnchor,
  setCursor,
  initCursor,
  setCursorPos,

// interface
  nullProc,
  clearHintInZone,
  assignHintToZone,
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

// timers
  clearTimer,
  initTimers,
  setTimerN,
  setTimer,
  checkTimers,

// controls
  invertZone,
  blankZone,
  clearWorkarea,
  clearStatus,
  doHintZone,
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
  BIN2BCD,

// keyboard
  __SCKeyVec,
  _modKey,
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