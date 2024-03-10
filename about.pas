{$LIBRARYPATH 'core/bin/'}

library about:$4000;

const
{$I 'data/data-mem.inc'}

{$I 'core/graph.h.inc'}
{$I 'core/interface.h.inc'}
{$I 'core/controls.h.inc'}

procedure showAbout();
begin
  // setmodule(-1);
  setScreenWidth(20);
  putText(10,6,'SPACE IMPACT EDITOR');
  putText(10,12,'-------------------');
  putText(15,24,'GSD 2024');
  putText(1,36,'GIT://GSOFTWAREDEVELOPMENT/SPACEIMPACT');
  setStatus({$I %DATE%});
End;

begin
  asm
    lda PORTB
    pha
    and #$FE
    sta PORTB
  end;
  showAbout();
  asm
    pla
    sta PORTB
  end;
end.