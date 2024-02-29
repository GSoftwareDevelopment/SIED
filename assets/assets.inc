const
_CONTROLS : Array of Byte = [{$BIN2CSV '../assets/bin/controls.bin'}];
   _ICONS : Array of Byte = [{$BIN2CSV '../assets/bin/icons.bin'}];
   _ICARD : Array of Byte = [{$BIN2CSV '../assets/bin/icon-card.bin'}];
   _IPATH : Array of Byte = [{$BIN2CSV '../assets/bin/icon-path.bin'}];

ACONTROLS : Array of Pointer = [
  @_CONTROLS[0],  @_CONTROLS[48], @_CONTROLS[144], @_CONTROLS[96], @_CONTROLS[192], @_CONTROLS[240],
  @_CONTROLS[24], @_CONTROLS[72], @_CONTROLS[168], @_CONTROLS[120],@_CONTROLS[216], @_CONTROLS[264]
];
AICONS : Array of Pointer = [
  @_ICONS[0],  @_ICONS[36],  @_ICONS[72],
  @_ICONS[108],@_ICONS[144], @_ICONS[180],
  @_ICONS[216],@_ICONS[252], @_ICONS[288]
];
