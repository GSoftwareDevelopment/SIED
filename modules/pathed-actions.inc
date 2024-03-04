procedure doSearchPath();
begin
  doInput(pathNameSearch);
  updateSearchField();
end;

procedure doSelectPath();
var
  name:PString;

begin
  name:=pathNamePtr[szone-LISTZONE];
  pathNameSearch:=name;
  updateSearchField();
end;

procedure doSetAction();
begin
  blank(0,0,7,6);
  putText(1,1,ACTION[szone-USER_ZONES_BEGIN]);
  invert(0,0,7,6);
  if pathNameSearch[0]=#0 then
  begin
    setStatus(HELP_BEFOREACTION);
  end;
end;

procedure doDelete();
begin
end;

procedure doEdit();
begin
end;
