procedure doSearchPath();
begin
  doInput(pathNameSearch);
  updateSearch();
end;

procedure doSelectPath();
var
  name:PString;

begin
  name:=pathNamePtr[szone-LISTZONE];
  pathNameSearch:=name;
  updateSearch();
end;

procedure doCreatePath();
begin
  clearWorkarea();

end;