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
    $38,$00,$10,$00,$80,$A0,$80,$00,$01,$05,$01,$00,$00,$10,$00,$38
  ];
  _LOOP:Array of byte = [ $A0, $82, $0A, $28 ];
  _PROCESS:Array of byte = [ $F5, $D7, $5F, $7D ];

  XVIEWPORT = 32;
  XCORNERS:Array[0..3] of byte = ( XVIEWPORT+44, XVIEWPORT, XVIEWPORT+94, XVIEWPORT+44 );
  YCORNERS:Array[0..3] of byte = (   0, 23, 23,  44 );

  _TOP    =  0;
  _MIDDLE = 24;
  _BOTTOM = 47;
  _LEFT   = -7;
  _CENTER = 39;
  _RIGHT  = 94;

  _EDGETOP    = 0;
  _EDGELEFT   = 1;
  _EDGERIGHT  = 2;
  _EDGEBOTTOM = 3;

  XEDGE:Array of shortint = [ _CENTER, _LEFT,   _RIGHT,  _CENTER ];
  YEDGE:Array of shortint = [ _TOP,    _MIDDLE, _MIDDLE, _BOTTOM ];

  EDGENAME:Array of string = [
    'TOP', 'LEFT', 'RIGHT', 'BOTTOM'
  ];

  pbit:Array[0..7] of byte = ( 128,64,32,16,8,4,2,1 );

  P4Mask:Array[0..3] of byte = ( %00111111, %11001111, %11110011, %11111100 );
  P4Color:Array[0..2,0..3] of byte = (
    ( %01000000, %00010000, %00000100, %00000001 ),
    ( %10000000, %00100000, %00001000, %00000010 ),
    ( %11000000, %00110000, %00001100, %00000011 )
  );

  TIMELINELENGTH = 128; // 128 pixels= 5s 03f (PAL 1/50)

  PT_CALCLEN          = %001;
  PT_UPDATETIMELINE   = %010;
  PT_UPDATEPATH       = %100;

var
  curSection:Shortint;
  curTrailLength:Smallint;
  adr:Pointer;
  TLPtr:pointer;
  TLPosition:SmallInt;
  TLShift:SmallInt;

  pathPattern:byte;

  v:Byte;
  px,py:shortint;
  dx,dy:shortint;
  ddx,ddy:shortint;
  loop:boolean;
  tm:Byte absolute $14;
  ntm:Byte;

function getTrailSize():word;
begin
  result:=word(pathListPtr[curTrailID])-word(pathListPtr[curTrailID+1]);
end;

// function getTrailLength():Smallint;
// var
//   adr:Pointer;

// begin
//   setCursor(_WAIT);
//   adr:=TLPtr;
//   TLPtr:=pathListPtr[curTrailID];
//   n:=pathStartEdge[curTrailID];
//   px:=XEDGE[n];
//   py:=YEDGE[n];
//   result:=0; ddx:=0; ddy:=0; loop:=false;
//   ntm:=tm+1;
//   repeat
//     if tm=ntm then
//     begin
//       ntm:=tm+1;
//       scr:=YSCR[56+48+16]+8;
//       for i:=0 to 2 do
//       begin
//         fillchar(scr,TIMELINELENGTH div 4,_PROCESS[i]);
//         inc(scr,40);
//       end;
//       asm
//         ldy adr._PROCESS+0
//         lda adr._PROCESS+1
//         sta adr._PROCESS+0
//         lda adr._PROCESS+2
//         sta adr._PROCESS+1
//         lda adr._PROCESS+3
//         sta adr._PROCESS+2
//         sty adr._PROCESS+3
//       end;
//     end;
//     v:=peek(TLPtr);
//     case v of
//       $80: // TR_JUMP
//         begin
//           inc(TLPtr);
//           TLPtr:=pointer(dpeek(TLPtr));
//           if (TLPtr<pathListPtr[curTrailID]) or (TLPtr>=pathListPtr[curTrailID+1]) then
//           begin
//             result:=word(TLPtr); break;
//           end;
//           if loop then
//           begin
//             if (ddx=0) and (ddy=0) then result:=-result;
//             break;
//           end;
//           loop:=true;
//           result:=0;
//         end;
//       $88: // TR_STOP
//         begin
//           break;
//         end;
//       $8F: // TR_KILL
//         begin
//           break;
//         end;
//     else
//       if v<>0 then
//       begin
//         dx:=v shr 4;  if (dx and %1000)<>0 then dx:=dx or %11110000;
//         dy:=v and $F; if (dy and %1000)<>0 then dy:=dy or %11110000;
//         inc(px,dx); inc(py,dy);
//         if ((px<_LEFT) or (px>_RIGHT)) or ((py<_TOP) or (py>_BOTTOM)) then
//           break;
//         inc(ddx,dx); inc(ddy,dy);
//       end;
//       inc(TLPtr);
//       inc(result);
//     end;
//   until false;
//   TLPtr:=adr;
//   setCursor(_ARROW);
// end;

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

  putTextC(0,63,'00:00.00');
  // putTextC(32,63,'00:00.00');

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
