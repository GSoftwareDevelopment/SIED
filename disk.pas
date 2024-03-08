{$define BASICOFF}
{$define ROMOFF}
{$define NOROMFONT}
{$LIBRARYPATH 'core/bin/'}

library disk:$4000;
uses cio;

const
{$I 'data/data-mem.inc'}

{$I 'core/pmg.var.inc'}

{$I 'core/assets.h.inc'}
{$I 'core/keyboard.h.inc'}
{$I 'core/cursor.h.inc'}
{$I 'core/graph.h.inc'}
{$I 'core/interface.h.inc'}
{$I 'core/controls.h.inc'}
{$I 'core/utils.h.inc'}

const
  MAXLISTITEMS = 10;
  LISTZONE = 16;
  ITEMSIZE = 14;
  MSG_READING = 'READING DIRECTORY...';
  MSG_SEEKING = 'SEEKING...';
  MSG_IOERR   = 'I/O ERROR $00';
  _HEX:array of char = '0123456789ABCDEF';
  _DIRFILEX:Array of Byte = [  9,  9,  9,  9,  9, 24, 24, 24, 24, 24 ];
  _DIRFILEY:Array of Byte = [ 13, 20, 27, 34, 41, 13, 20, 27, 34, 41 ];

var
  dev:string[4] absolute $04C0; //3f00;
  fn:string[16] absolute $04C5; //3f05;
  _fn:string[20] absolute $04D7; //3f17;
  filemask:string[16] absolute $04EC; //3f2C;
  dirName:Array[0..MAXLISTITEMS-1] of string[12];
  dirPageBegin:smallint;

//
procedure readDirectory(); Forward;
procedure updateFileNameField(); Forward;
{$I 'disk-actions.inc'}
{$I 'disk-controls.inc'}

procedure showDiskDirectory();
begin
  addShortcutKey(k_D,@keyDevice);
  addShortcutKey(k_F,@keyFileName);
  addShortcutKey(k_L,@keyLoad);
  addShortcutKey(k_S,@keySave);
  addShortcutKey(k_E,@keyExport);
  addShortcutKey(k_I,@keyImport);
  addShortcutKey(k_UP,@keySelectFile);
  addShortcutKey(k_DOWN,@keySelectFile);
  addShortcutKey(k_LEFT,@keySelectFile);
  addShortcutKey(k_RIGHT,@keySelectFile);
  addShortcutKey(k_CLEAR,@doEraseFileName);
  addShortcutKey(k_RETURN,@doChoiceFile);
  setScreenWidth(20);
  fillchar(YSCR[56+8]+4,16,$FF);
  fillchar(Pointer(PMG_ADDR+$300+23),50,$FF);
  HPOSP[2]:=44; PCOL[2]:=$E6; SIZEP[2]:=%11;
  putImage(_IDISK,0,0,3,48);
  putImage(_VSCROLL,19,11,1,3);
  putImage(_VSCROLL+2,19,44,1,3);
  putImage(_ERASEINPUT,19,1,1,5);
  addZoneN(LISTZONE-1,38,10,2,5,@doPrevPageDir);
  addZoneN(LISTZONE+MAXLISTITEMS,38,43,2,5,@doNextPageDir);

  addZoneN(9 ,0,0 ,6,12,@nullProc);
  addZoneN(10,0,12,6,12,@nullProc);
  addZoneN(11,0,24,6,12,@nullProc);
  addZoneN(12,0,36,6,12,@nullProc);
  putText(9,1,'DEV');
  putText(20,1,'FILE');
  addInput(13,1,4,dev,@doDevice);
  addInput(25,1,12,fn,@doFilename);
  addZoneN(31,38,0,2,7,@doEraseFileName);
  startDirectory();
End;

exports showDiskDirectory;

var
  moduleInitialized:Byte absolute $62;

begin
  if (moduleInitialized and $1=0) then
  begin
    filemask:='*.*'#$9B;
    dev:='D:';
    fn:='NONAME';
    moduleInitialized:=moduleInitialized or $1;
  end;
end.