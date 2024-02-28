const
  _HEX:array of char = '0123456789ABCDEF';

var
  dev:string[4] absolute $400;
  fn:string[16] absolute $405;
  _fn:string[20] absolute $417;
  filemask:string[16] absolute $42C;
  dirPageBegin:smallint;

procedure doChoiceFile();
begin
  case szone of
    14..29: begin
      fn:=dirName[szone-14];
      // set filename in field
      blank(7,1,12,5);
      putText(7,1,fn);
    End;
  End;
End;

procedure readDirectory(); Forward;

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

procedure readDirectory;
var
  n,j:Shortint;
  dirSeek:smallint;

begin
  if dev[1]='D' then
  begin
    setPivot(3,3); setCursor(@_WAIT);
    _fn:=dev; move(filemask[1],_fn[Byte(_fn[0])+1],Byte(filemask[0]));
    dirSeek:=dirPageBegin;
    if dirPageBegin>0 then setStatus('SEEKING...') else setStatus('READING DIRECTORY...');
    opn(1,6,0,_fn);
    n:=-1;
    while (IOResult=1) and (n<15) do
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
      if (n=-1) then setStatus('READING DIRECTORY...');

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
    _fn:='I/O ERROR $00';
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
      blank(27,41,12,5); clearZone(28);
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
  setPivot(0,0); setCursor(@_ARROW);
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
  if curModule=0 then exit;
  setModule(0);
  fillchar(YSCR[56+9],20,$FF);
  setScreenWidth(20);
  addInput(1,1,4,dev,@doDevice);
  addInput(7,1,12,fn,@doFilename);
  addButton(21,1,'LOAD',@nullProc);
  addButton(27,1,'SAVE',@nullProc);
  addButton(33,1,'EXPORT',@nullProc);
  startDirectory();
End;

