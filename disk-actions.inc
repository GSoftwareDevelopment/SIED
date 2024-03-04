procedure doChoiceFile();
begin
  case szone of
    LISTZONE..LISTZONE+MAXLISTITEMS-1: begin
      fn:=dirName[szone-LISTZONE];
      // set filename in field
      blank(16,1,12,5);
      putText(16,1,fn);
    End;
  End;
End;

procedure doPrevPageDir();
begin
  dec(dirPageBegin,MAXLISTITEMS);
  readDirectory();
End;

procedure doNextPageDir();
begin
  inc(dirPageBegin,MAXLISTITEMS);
  readDirectory();
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

procedure toZone(n:byte);
begin
  szone:=n; setCursorInZone(n);
end;

procedure keyDevice();
begin
  toZone(13); doDevice();
end;

procedure keyFileName();
begin
  toZone(14); doFilename();
end;

procedure keyLoad();
begin
  toZone(10);
end;

procedure keySave();
begin
  toZone(9);
end;

procedure keyExport();
begin
  toZone(11);
end;

procedure keyImport();
begin
  toZone(12);
end;

procedure keyPrevPageDir();
begin
  toZone(LISTZONE-1); doPrevPageDir();
end;

procedure keyNextPageDir();
begin
  toZone(LISTZONE+MAXLISTITEMS); doNextPageDir();
end;
