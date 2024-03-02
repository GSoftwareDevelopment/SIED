{$define BASICOFF}
{$define ROMOFF}
{$define NOROMFONT}
{$LIBRARYPATH 'core/bin/'}

library disk:$4000;
uses cio;

const
{$I 'data/data-mem.inc'}

{$I 'core/keyboard.var.inc'}

{$I 'core/cursor.h.inc'}
{$I 'core/graph.h.inc'}
{$I 'core/interface.h.inc'}
{$I 'core/controls.h.inc'}
{$I 'core/utils.h.inc'}

const
  MSG_READING = 'READING DIRECTORY...';
  MSG_SEEKING = 'SEEKING...';
  MSG_IOERR   = 'I/O ERROR $00';
  _HEX:array of char = '0123456789ABCDEF';
  _DIRFILEX:Array of Byte = [  1,  1,  1,  1,  1, 14, 14, 14, 14, 14, 27, 27, 27, 27, 27 ];
  _DIRFILEY:Array of Byte = [ 13, 20, 27, 34, 41, 13, 20, 27, 34, 41, 13, 20, 27, 34, 41 ];

var
  dev:string[4] absolute $04C0; //3f00;
  fn:string[16] absolute $04C5; //3f05;
  _fn:string[20] absolute $04D7; //3f17;
  filemask:string[16] absolute $04EC; //3f2C;
  dirPageBegin:smallint;
  dirName:Array[0..14] of string[12];
  // dirType:Array[0..14] of Byte;

//
procedure readDirectory(); Forward;

procedure addFileLabel(n:Byte; prc:TZoneProc);
var
  x,y:Byte;

begin
  if n>14 then exit;
  x:=_DIRFILEX[n]; y:=_DIRFILEY[n];
  putText(x,y,dirName[n]);
  addZoneN(14+n,x-1,y-1,14,7,prc);
End;

procedure doChoiceFile();
begin
  case szone of
    14..28: begin
      fn:=dirName[szone-14];
      // set filename in field
      blank(7,1,12,5);
      putText(7,1,fn);
    End;
  End;
End;

procedure doPrevPageDir();
begin
  dec(dirPageBegin,14);
  readDirectory();
End;

procedure doNextPageDir();
begin
  inc(dirPageBegin,14);
  readDirectory();
End;

procedure getLn(chn:Byte; buf:PString); Register; Assembler; Asm icl 'asm/get_line.a65' End;

procedure readDirectory;
var
  n,j:Shortint;
  dirSeek:smallint;

begin
  if dev[1]='D' then
  begin
    setPivot(3,3); setCursor(_WAIT);
    _fn:=dev; move(filemask[1],_fn[Byte(_fn[0])+1],Byte(filemask[0]));
    dirSeek:=dirPageBegin;
    if dirPageBegin>0 then setStatus(MSG_SEEKING) else setStatus(MSG_READING);
    opn(1,6,0,_fn);
    n:=-1;
    while (IOResult=1) and (n<14) do
    begin
      getLn(1,_fn);
      if (_fn[1]>='0') then
      begin
        _fn[0]:=char(Byte(_fn[0])-1); setStatus(_fn);
        IOResult:=3;
        break;
      End;
      if dirSeek>0 then
      begin
        dec(dirSeek);
        continue;
      End;
      if (n=-1) then setStatus(MSG_READING);

      // isDir:=((_fn[2]=':') or (_fn[18]='>'));
      inc(n);
      reduceFileName(_fn,dirName[n]);
    End;
  End
  else
    IOResult:=160;
  fillchar(YSCR[56+10],38*20,0);
  if IOResult>3 then
  begin
    _fn:=MSG_IOERR;
    n:=IOResult shr 4; _fn[12]:=_HEX[n];
    n:=IOResult and 15; _fn[13]:=_HEX[n];
    setStatus(_fn);
  End
  else
  begin
    for j:=14 to 30 do clearZone(j);
    j:=0; while (j<=n) do
    begin
      addFileLabel(j,@doChoiceFile);
      inc(j);
    End;
    if (dirPageBegin>0) or (IOResult<3) then
    begin
      blank(27,41,12,5); // clear last entry
      clearZone(28); // clear last entry zone
      if dirPageBegin>0 then
        addButton(27,41,'PREV',@doPrevPageDir);
      if IOResult<3 then
      begin
        addButton(35,41,'NEXT',@doNextPageDir);
        clearStatus();
      End;
    End;
  End;
  cls(1);
  setPivot(0,0); setCursor(_ARROW);
End;

procedure startDirectory;
begin
  dirPageBegin:=0;
  readDirectory();
End;

procedure doFilename();
begin
  doInput(fn);
End;

procedure doDevice();
begin
  if doInput(dev)>=0 then startDirectory();
End;

procedure showDiskDirectory();
begin
  fillchar(YSCR[56+9],20,$FF);
  setScreenWidth(20);
  addInput(1,1,4,dev,@doDevice);
  addInput(7,1,12,fn,@doFilename);
  addButton(21,1,'LOAD',@nullProc);
  addButton(27,1,'SAVE',@nullProc);
  addButton(33,1,'EXPORT',@nullProc);
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