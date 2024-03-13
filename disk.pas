{$define BASICOFF}
{$define ROMOFF}
{$define NOROMFONT}
{$LIBRARYPATH 'bin/'}

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

  _IDISK:Array of byte = [
    $00,$00,$00,$00,$FF,$C0,$00,$81,$C0,$08,$81,$80,$0C,$FF,$C0,$0E,$F3,$C0,$0C,$FF,$C0,$08,$F3,$C0,$00,$F3,$C0,$00,$F3,$C0,$00,$FF,$C0,$00,$00,$00,$00,$00,$00,$07,$FE,$00,$04,$0E,$00,$04,$0C,$80,$07,$FE,$C0,$07,$9E,$E0,$07,$FE,$C0,$07,$9E,$80,$07,$9E,$00,$07,$9E,$00,$07,$FE,$00,$00,$00,$00,$00,$00,$00,$00,$80,$00,$01,$D7,$80,$00,$F7,$80,$00,$77,$80,$00,$F7,$80,$00,$07,$80,$00,$7F,$80,$00,$7F,$80,$00,$40,$80,$00,$3F,$00,$00,$00,$00,$00,$00,$00,$00,$0F,$00,$01,$E7,$00,$01,$CF,$00,$01,$DD,$00,$01,$C8,$00,$01,$E2,$00,$01,$FE,$00,$01,$FE,$00,$01,$02,$00,$00,$FC,$00,$00,$00,$00
  ];

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
  dta k_RETURN,a(MAIN.doChoiceFile)
end;

procedure showDiskDirectory();
begin
  registerShortcutKeys(@shortKeys,12);
  setScreenWidth(20);
  fillchar(YSCR[56+8]+4,16,$FF);
  putImage(@_IDISK,0,0,3,48);
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
  HPOSP[2]:=44; PCOL[2]:=$E4; SIZEP[2]:=%11;
  clearAllShortcutsKey();
  showDiskDirectory();
  asm
    pla
    sta PORTB
  end;
end.