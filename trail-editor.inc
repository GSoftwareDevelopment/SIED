{$I 'core/cursor.h.inc'}
const
  _ITRLED :Array of byte = [
    $00,$00,$00,$00,$00,$C0,$10,$00,$60,$2F,$82,$20,$40,$A3,$60,$2F,$A1,$F0,$10,$20,$38,$07,$E0,$14,$08,$00,$08,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$0E,$EE,$B0,$0A,$88,$A8,$0C,$CA,$A8,$0A,$8A,$A8,$0E,$EE,$A8,$00,$00,$00,$00,$1C,$00,$00,$3E,$00,$00,$1C,$00,$00,$08,$00,$00,$00,$00,$00,$08,$00,$00,$00,$00,$00,$10,$00,$05,$40,$00,$10,$00,$00,$00,$01,$40,$20,$14,$10,$01,$40,$00,$14,$00,$10,$00,$05,$40,$00,$10,$00,$00,$00,$00,$00,$00,$00,$00,$38,$00,$00,$7C,$00,$00,$38,$00,$00,$10,$00,$00,$00,$00,$1D,$64,$D4,$11,$55,$14,$19,$54,$9C,$11,$54,$54,$11,$55,$94,$00,$00,$00
  ];
  _ISELECT4 :Array of byte = [
    $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,
    $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,
    $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,
    $80,$80,$80,$80,$80,$80,$80,$80,$80,$80,$80,$80,
    $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,
    $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,
    $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
  ];
  _CORNERS:Array of byte = [
    $F0,$80,$A0,$80,$0F,$00,$06,$00,$0F,$01,$05,$01,$80,$A0,$A0,$80,$01,$05,$05,$01,$80,$A0,$80,$F0,$00,$06,$00,$0F,$01,$05,$01,$0F
  ];
  CORNERSX:Array[0..7] of byte = ( 72,106,144, 72,144, 72,106,144 );
  CORNERSY:Array[0..7] of byte = (  0,  0,  0, 23, 23, 44, 44, 44 );

var
  curSection:Shortint;

function getTrailSize():word;
begin
  result:=word(pathListPtr[curTrailID])-word(pathListPtr[curTrailID+1]);
end;

// procedure clearSection(); Forward;
// procedure updatePathSection(); Forward;
// procedure updateGameScreen(); Forward;
procedure doSwitchPathSection(); Forward;
// procedure addCorrner(p:pointer; x,y:byte; prc:TZoneProc); Forward;
{$I 'trail-editor-controls.inc'}
{$I 'trail-editor-actions.inc'}

procedure showTrailEditor();
begin
  clearWorkarea();
  clearStatus();
  setControls(%001111); // only Prev/Play/Stop/Next controls
  setControl(-1);
  setStatus('EDIT:');
  putTextC(6,69,pathNameSearch);
  for i:=56+48+16 to 56+48+18 do fillchar(Pointer(word(YSCR[i])+8),32,%01010101);
  putTextC(0,63,'00:00.00');

  SetScreenWidth(20);
  putImage(@_ITRLED,0,0,3,48);

   addZone(0,0,3,12,@doBack2Selector);      // Back to trail selector
  addZoneH(@nullProc);                      // Options
   addZone(0,12,6,12,@doSwitchPathSection); // Begin trail
  addZoneV(@doSwitchPathSection);           // Path
  addZoneV(@doSwitchPathSection);           // Finish trail

  curSection:=-1;
  if getTrailSize=0 then
    szone:=11   // show Begin path
  else
    szone:=12;  // show trail path
  doSwitchPathSection();

End;
