// {$define BASICOFF}
// {$define ROMOFF}
// {$define NOROMFONT}
library core:$d800;

const
{$I '../data/data-mem.inc'}

{$I '../assets/assets.inc'}
{$I 'keyboard.var.inc'}
{$I '../utils.inc'}

var
  tm:Byte absolute $14;
  i:shortint absolute $3e;

{$I 'graph.inc'}
procedure invertZone(i:Shortint); Forward;
procedure setIcon(n:Shortint); Forward;
procedure setControl(n:Shortint); Forward;

{$I 'interface.inc'}
{$I 'controls.inc'}

exports
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
  checkZones,
// controls assets
  _CONTROLS,
  _ICONS,
  _ICARD,
  _IPATH,
  ACONTROLS,
  AICONS,

// controls
  invertZone,
  clearWorkarea,
  clearStatus,
  setStatus,
  setIcon,
  setControls,
  setControl,
  addButton,
  addInput,
  doInput;

end.