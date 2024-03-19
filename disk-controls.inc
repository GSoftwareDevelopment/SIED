procedure showIOError();
begin
  _fn:=MSG_IOERR;
  _fn[12]:=_HEX[IOResult shr 4];
  _fn[13]:=_HEX[IOResult and 15];
  setStatus(_fn);
End;

procedure updateFileNameField();
begin
  blank(25,1,12,5);
  putText(25,1,fn);
end;

procedure addFileLabel(n:Byte; prc:TZoneProc);
var
  x,y:Byte;
  name:PString;
  dirSign:byte;

begin
  if n>LISTZONE+MAXLISTITEMS-1 then exit;
  x:=_DIRFILEX[n]; y:=_DIRFILEY[n];
  name:=dirName[n]; dirSign:=byte(name[1]);
  if (dirSign and $80)<>0 then
  begin
    putChar(x-1,y,'<');
    putChar(x+byte(name[0]),y,'>');
  end;
  putText(x,y,name);
  addZoneN(LISTZONE+n,x-1,y-1,ITEMSIZE,7,prc);
End;

procedure getLn(chn:Byte; buf:PString); Register; Assembler; Asm icl 'core/asm/get_line.a65' End;

procedure readDirectory;
var
  n,j:Shortint;
  totalFiles:smallint;
  name:PString;
  isDir:Boolean;

begin
  if dev[1]='D' then
  begin
    setCursor(_WAIT);
    _fn:=dev; move(filemask[1],_fn[Byte(_fn[0])+1],Byte(filemask[0]));
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
  End
  else
    IOResult:=160;
  if IOResult>3 then
    showIOError()
  else
  begin
    setScreenWidth(20);
    for j:=0 to MAXLISTITEMS-1 do
    begin
      blank(_DIRFILEX[j]-1,_DIRFILEY[j]-1,ITEMSIZE,6);
      clearZone(LISTZONE+j);
    end;
    j:=0; while (j<=n) do
    begin
      addFileLabel(j,@doChoiceFile);
      inc(j);
    End;
    _mZoneActive[LISTZONE-1]:=(dirPageBegin>0);
    _mZoneActive[LISTZONE+MAXLISTITEMS]:=((dirPageBegin+MAXLISTITEMS)<totalFiles);
    if IOResult<3 then clearStatus();
  End;
  cls(1);
  setCursor(_ARROW);
End;
