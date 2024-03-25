{$define BASICOFF}
{$define ROMOFF}
{$define NOROMFONT}
{$LIBRARYPATH 'bin/'}
{$UNITPATH 'units/'}

library disk:$4000;
uses cio,rle;

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

  _IDISK:Array of byte = [
    $00,$00,$00,$00,$FF,$C0,$00,$81,$C0,$08,$81,$80,$0C,$FF,$C0,$0E,$F3,$C0,$0C,$FF,$C0,$08,$F3,$C0,$00,$F3,$C0,$00,$F3,$C0,$00,$FF,$C0,$00,$00,$00,$00,$00,$00,$07,$FE,$00,$04,$0E,$00,$04,$0C,$80,$07,$FE,$C0,$07,$9E,$E0,$07,$FE,$C0,$07,$9E,$80,$07,$9E,$00,$07,$9E,$00,$07,$FE,$00,$00,$00,$00,$00,$00,$00,$00,$80,$00,$01,$D7,$80,$00,$F7,$80,$00,$77,$80,$00,$F7,$80,$00,$07,$80,$00,$7F,$80,$00,$7F,$80,$00,$40,$80,$00,$3F,$00,$00,$00,$00,$00,$00,$00,$00,$0F,$00,$01,$E7,$00,$01,$CF,$00,$01,$DD,$00,$01,$C8,$00,$01,$E2,$00,$01,$FE,$00,$01,$FE,$00,$01,$02,$00,$00,$FC,$00,$00,$00,$00
  ];

  MAXLISTITEMS = 10;
  LISTZONE = 16;
  ITEMSIZE = 14;
  MSG_READING = 'READING DIRECTORY...';
  MSG_SEEKING = 'SEEKING...';
  MSG_SAVE = 'SAVEING...';
  MSG_LOAD = 'LOADING...';
  MSG_LOAD_SUCCESS = 'LOADED SUCCESSFULLY';
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
  n:shortint;
  j:Shortint;
  totalFiles:smallint;

  rlebuf:Array[0..RLEBUFFERSIZE-1] of byte absolute RLEBUFFER_ADDR;
  src,dst:Pointer;
  blockSize:Word;

//

procedure prepareFileName(f:PString);
begin
  fillchar(@_fn,21,$9b);
  _fn:=dev; move(f[1],_fn[Byte(_fn[0])+1],Byte(f[0]));
end;

procedure getLn(chn:Byte; buf:PString); Register; Assembler; Asm icl 'core/asm/get_line.a65' End;

function getDirectory():Byte;
var
  name:PString;
  isDir:Boolean;

begin
  if dev[1]<>'D' then exit(160);
  setCursor(_WAIT);
  prepareFileName(filemask);
  totalFiles:=0;
  if dirPageBegin>0 then setStatus(MSG_SEEKING) else setStatus(MSG_READING);
  opn(1,6,0,_fn);
  n:=-1;
  while (IOResult=1) do
  begin
    getLn(1,_fn);
    if (_fn[1]>='0') then
    begin
      _fn[0]:=char(Byte(_fn[0])-1); setStatus(_fn);
      IOResult:=3;
      break;
    End;
    inc(totalFiles);
    if totalFiles>=dirPageBegin then
    begin
      if (n=-1) then setStatus(MSG_READING);
      if (n<MAXLISTITEMS-1) then
      begin
        inc(n);
        name:=dirName[n];
        isDir:=((_fn[2]=':') or (_fn[14]='>'));
        reduceFileName(_fn,name);
        if isDir then
          name[1]:=char(byte(name[1]) or $80);
      End;
    End;
  End;
  result:=IOResult;
  cls(1);
  setCursor(_ARROW);
end;

function loadFromDisk(fn:PString):byte;
begin
  setCursor(_WAIT);
  prepareFileName(fn);
  opn(1,4,0,_fn);
  if IOResult=1 then
  begin
    dst:=pointer(CARD_ADDR);
    repeat
      BGet(1,@blockSize,2);
      BGet(1,@rlebuf,blockSize);
      blockSize:=RLEDecompress(@rlebuf,dst,blockSize);
      inc(dst,blockSize);
    until word(dst)=$d000;
  end;
  result:=IOResult;
  cls(1);
  setCursor(_ARROW);
end;

function saveToDisk(fn:PString):Byte;
begin
  setCursor(_WAIT);
  prepareFileName(fn);
  opn(1,8,0,_fn);
  if IOResult=1 then
  begin
    src:=pointer(CARD_ADDR);
    repeat
      blockSize:=RLECompress(src,@rlebuf,RLEBUFFERSIZE);
      BPut(1,@blockSize,2);
      BPut(1,@rlebuf,blockSize);
      inc(src,RLEBUFFERSIZE);
    until word(src)=$D000;
  end;
  result:=IOResult;
  cls(1);
  setCursor(_ARROW);
end;

//
procedure showIOError(); Forward;
procedure updateDirList(); Forward;
procedure updateFileNameField(); Forward;
{$I 'disk-actions.inc'}
{$I 'disk-controls.inc'}

procedure shortKeys(); assembler;
asm
  dta k_D,a(MAIN.keyDevice)
  dta k_F,a(MAIN.keyFileName)
  dta k_L,a(MAIN.keyLoad)
  dta k_S,a(MAIN.keySave)
  dta k_E,a(MAIN.keyExport)
  dta k_I,a(MAIN.keyImport)
  dta k_UP,a(MAIN.keySelectFile)
  dta k_DOWN,a(MAIN.keySelectFile)
  dta k_LEFT,a(MAIN.keySelectFile)
  dta k_RIGHT,a(MAIN.keySelectFile)
  dta k_CLEAR,a(MAIN.doEraseFileName)
  // dta k_RETURN,a(MAIN.doChoiceFile)
end;

procedure showDiskDirectory();
begin
  registerShortcutKeys(@shortKeys,11);
  registerTabControl();
  setScreenWidth(20);
  fillchar(YSCR[56+8]+4,16,$FF);
  putImage(@_IDISK,0,0,3,48);
  putImage(_VSCROLL,19,11,1,3);
  putImage(_VSCROLL+2,19,44,1,3);
  putImage(_ERASEINPUT,19,1,1,5);
  addZoneN(LISTZONE-1,38,10,2,5,@doPrevPageDir);
  addZoneN(LISTZONE+MAXLISTITEMS,38,43,2,5,@doNextPageDir);

  n:=addZoneN(9 ,0,0 ,6,12,@doSaveToDisk);
  assignHintToZone(n,'SAVE ON DISK');
  n:=addZoneN(10,0,12,6,12,@doLoadFromDisk);
  assignHintToZone(n,'LOAD FROM DISK');
  n:=addZoneN(11,0,24,6,12,@nullProc);
  assignHintToZone(n,'EXPORT TO CARTRIDGE');
  n:=addZoneN(12,0,36,6,12,@nullProc);
  assignHintToZone(n,'IMPORT FROM CARTRIDGE');
  putText(9,1,'DEV');
  putText(20,1,'FILE');
  n:=addInput(13,1,4,dev,@doDevice); zoneTabIndex[n]:=1;
  n:=addInput(25,1,12,fn,@doFilename); zoneTabIndex[n]:=2;
  addZoneN(31,38,0,2,7,@doEraseFileName);
  startDirectory();
End;

var
  moduleInitialized:Byte absolute $62;

begin
  asm
    lda PORTB
    pha
    and #$FE
    sta PORTB
  end;
  if (moduleInitialized and $1=0) then
  begin
    filemask:='*.*'#$9B;
    dev:='D:';
    fn:='NONAME';
    moduleInitialized:=moduleInitialized or $1;
  end;
  fillchar(Pointer(PMG_ADDR+$300+23),50,$FF);
  HPOSP[2]:=44; PCOL[2]:=BASE_COLOR+$02; SIZEP[2]:=%11;
  clearAllShortcutsKey();
  showDiskDirectory();
  asm
    pla
    sta PORTB
  end;
end.