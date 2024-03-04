procedure addFileLabel(n:Byte; prc:TZoneProc);
var
  x,y:Byte;

begin
  if n>LISTZONE+MAXLISTITEMS-1 then exit;
  x:=_DIRFILEX[n]; y:=_DIRFILEY[n];
  putText(x,y,dirName[n]);
  addZoneN(LISTZONE+n,x-1,y-1,14,7,prc);
End;

procedure getLn(chn:Byte; buf:PString); Register; Assembler; Asm icl 'core/asm/get_line.a65' End;

procedure readDirectory;
var
  n,j:Shortint;
  dirSeek:smallint;

begin
  if dev[1]='D' then
  begin
    setCursorShapeAnchor(3,3); setCursorShape(_WAIT);
    _fn:=dev; move(filemask[1],_fn[Byte(_fn[0])+1],Byte(filemask[0]));
    dirSeek:=dirPageBegin;
    if dirPageBegin>0 then setStatus(MSG_SEEKING) else setStatus(MSG_READING);
    opn(1,6,0,_fn);
    n:=-1;
    while (IOResult=1) and (n<MAXLISTITEMS-1) do
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
  if IOResult>3 then
  begin
    _fn:=MSG_IOERR;
    n:=IOResult shr 4; _fn[12]:=_HEX[n];
    n:=IOResult and 15; _fn[13]:=_HEX[n];
    setStatus(_fn);
  End
  else
  begin
    for j:=LISTZONE to LISTZONE+MAXLISTITEMS-1 do
    begin
      blankZone(j);
      clearZone(j);
    end;
    j:=0; while (j<=n) do
    begin
      addFileLabel(j,@doChoiceFile);
      inc(j);
    End;
    if (dirPageBegin>0) or (IOResult<3) then
    begin
      _mZoneActive[LISTZONE-1]:=(dirPageBegin>0);
      _mZoneActive[LISTZONE+MAXLISTITEMS]:=IOResult<3;
      clearStatus();
    End;
  End;
  cls(1);
  setCursorShapeAnchor(0,0); setCursorShape(_ARROW);
End;
