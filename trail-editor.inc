{$I 'core/cursor.h.inc'}
{$I 'core/timers.h.inc'}

{$I 'trail-editor.const.inc'}

var
  curSection:Shortint;
  curTrailLength:Smallint;
  adr:Pointer;
  TLPtr:pointer;
  TLPosition:SmallInt;
  curTLPos:SmallInt;
  TLShift:SmallInt;

  animTimer:shortint;
  trailPattern,
  loopPattern:longint;

  v:Byte;
  px,py:shortint;
  curPx,curPy:shortint;
  dx,dy:shortint;
  curDx,curDy:shortint;

  _curPx,_curPy:shortint;
  _curDx,_curDy:shortint;

  ddx,ddy:shortint;
  loop:boolean;
  tm:Byte absolute $14;
  ntm:Byte;

  min,sec,frm,bcd:Byte;
  s:string[8];
  cursorInViewPort:Boolean;

//
//
//

function getTrailSize():word;
begin
  result:=word(pathListPtr[curTrailID])-word(pathListPtr[curTrailID+1]);
end;

procedure point(x,y:Shortint; c:Byte);
var
  bit:byte;

begin
  bit:=x and 7; x:=(8+x) shr 3;
  scr:=YSCR[4+y]+x;
  bit:=pbit[bit];
  if c=0 then
    poke(scr,peek(scr) and not bit)
  else
    poke(scr,peek(scr) or bit);
end;

procedure TLPoint(c:Byte);
var
  m,b:Byte;
  pos:Smallint;
  bit:Byte;

begin
  pos:=TLPosition-TLShift;
  if (pos>=0) and (pos<TIMELINELENGTH) then
  begin
    TLPtr:=YSCR[56+48+16]+(XTIMELINE div 4)+pos shr 2;
    bit:=pos and 3;
    m:=P4Mask[bit]; if c>0 then b:=P4Color[c-1][bit] else b:=0;
    poke(TLPtr,(peek(TLPtr) and m) or b); inc(TLPtr,40);
    poke(TLPtr,(peek(TLPtr) and m) or b); inc(TLPtr,40);
    poke(TLPtr,(peek(TLPtr) and m) or b);
    if bit=3 then inc(TLPtr);
  end;
end;

function BIN2BCD(bin:Byte):Byte; Assembler;
asm
  lda #0
  sta result+0

loop8Bits:
  ldy #{sed}
  asl bin
  adc result+0
  sta result+0
  iny
  bne loop8Bits+1
  cld
end;

//
//
//

const
  SK_COUNT = 3;
procedure shortKeys(); Assembler; Forward;

procedure doSwitchPathSection(); Forward;
procedure showPathBegin(); Forward;
procedure showPathInner(); Forward;
procedure showPathFinish(); Forward;
{$I 'trail-editor-controls.inc'}
{$I 'trail-editor-actions.inc'}

procedure shortKeys(); Assembler;
asm
  dta k_COMMA,a(MAIN.doTLPrevEvent)
  dta k_DOT,  a(MAIN.doTLNextEvent)
  dta k_V,    a(MAIN.doEditPath)
end;

procedure showPathBegin();
begin
  updateGameScreen();
  updateStartPoint(SP_DESCRIPTION);
  if (curSection=0) or (pathStartEdge[curTrailID]=-128) then
  begin
    addEdges(@doSetBeginEdge);
    putText((XVIEWPORT div 4)+5,30,'PLEASE DEFINE IT');
    putText((XVIEWPORT div 4)+6,36,'BY CHOICE EDGE')
  end
  else
    addButton((XVIEWPORT div 4)+6,36,'REDEFINE IT...',@doRedefineStartPoint);
end;

procedure showPathInner();
begin

  putImage(@_ITRLTL,18,0,2,48);
   addZone(36,0,4,12,@nullProc); // Add point
  addZoneV(@nullProc);           // Delete point
  addZoneV(@nullProc);           // horizontal mirror
  addZoneV(@nullProc);           // ?

  updateGameScreen();

  _curPx:=-128; _curPy:=-128;
  _curDx:=-128; _curDy:=-128;

  addZoneN(ZONE_TIMELINE,8,65,32,7,@doSetTimeLinePos);
  _mZoneInCall[ZONE_TIMELINE]:=@inTimeLine;

  addZoneN(ZONE_VIEWPORT,XVIEWPORT div 4,0,26,48,@doEditPath);
  _mZoneInCall[ZONE_VIEWPORT]:=@inGameScreen;

  HPOSP[3]:=180;
  HPOSM[0]:=48+XTIMELINE+curTLPos-TLShift;
  trailPattern:=%111111111111111111111111;
    loopPattern:=%100100100100100100100100;
  curTrailLength:=processTrail(PT_CALCLEN+PT_UPDATETIMELINE);
  onTimerAnim();
  registerShortcutKeys(@shortKeys,SK_COUNT);
end;

procedure showPathFinish();
begin
end;

//
//
//

procedure showTrailEditor();
begin
  clearWorkarea();
  clearStatus();
  setControls(%001111); // only Prev/Play/Stop/Next controls
  setControl(-1);
  setStatus('EDIT:');
  putTextC(6,69,pathNameSearch);

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

  TLShift:=0; curTLPos:=0;
  doSwitchPathSection();
End;
