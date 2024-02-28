procedure showAbout();
begin
  setmodule(-1);
  setScreenWidth(20);
  putText(10,6,'SPACE IMPACT EDITOR');
  putText(10,12,'-------------------');
  putText(15,24,'GSD 2024');
  putText(1,36,'GIT://GSOFTWAREDEVELOPMENT/SPACEIMPACT');
  setStatus({$I %DATE%});
End;
