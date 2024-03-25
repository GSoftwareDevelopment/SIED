{$I 'core/cursor.h.inc'}
{$I 'core/timers.h.inc'}

{$I 'trail-editor.const.inc'}

var
  curSection:Shortint;
  trailLength:Smallint;
  totalTrailLength:Smallint;
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

  curTool:Byte;
  SK_curTool:Pointer;
  SKcount_curTool:Byte;

procedure registerToolShortKeys(SK:Pointer; count:Byte);
begin
  SK_curTool:=SK; SKcount_curTool:=count;
  registerShortcutKeys(SK,count);
end;

procedure unregisterToolShortKeys();
begin
  if (SK_curTool<>nil) and (SKcount_curTool<>0) then
  begin
    unregisterShortcutKeys(SK_curTool,SKcount_curTool);
    SK_curTool:=nil; SKcount_curTool:=0;
  end;
end;

//
//
//

procedure point(x,y:Shortint; c:Byte);
var
  bit:byte;

begin
  if (y<0) or (y>47) then exit;
  bit:=x and 7; x:=(8+x) shr 3;
  scr:=YSCR[4+y]+x;
  bit:=PBIT[bit];
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
    TLPtr:=Pointer(TLBUF_ADDR+TIMELINE_SIZEB+pos shr 2);
    bit:=pos and 3;
    m:=P4MASK[bit]; if c>0 then b:=P4COLOR[c-1][bit] else b:=0;
    poke(TLPtr,(peek(TLPtr) and m) or b); inc(TLPtr,TIMELINE_SIZEB);
    poke(TLPtr,(peek(TLPtr) and m) or b); inc(TLPtr,TIMELINE_SIZEB);
    poke(TLPtr,(peek(TLPtr) and m) or b);
  end;
end;

//
//
//

const
  SK_INNER_COUNT      = 6;
  SK_GLOBAL_COUNT     = 3;
  SK_MOVEPOINT_COUNT  = 4;

procedure movePointShortKeys(); Assembler; Forward; Keep;
procedure globalShortKeys(); assembler; Forward;
procedure innerPathShortKeys(); Assembler; Forward;

procedure doSwitchPathSection(); Forward;
procedure showPathBegin(); Forward;
procedure showPathInner(); Forward;
procedure showPathFinish(); Forward;
{$I 'trail-editor-controls.inc'}
{$I 'trail-editor-actions.inc'}

procedure globalShortKeys(); assembler;
asm
  dta k_B,        a(MAIN.keySwitch2BeginPath)
  dta k_I,        a(MAIN.keySwitch2InnerPath)
  dta k_F,        a(MAIN.keySwitch2FinishPath)
end;

procedure innerPathShortKeys(); Assembler;
asm
  dta k_COMMA,    a(MAIN.doTLPrevEvent)
  dta k_DOT,      a(MAIN.doTLNextEvent)
  dta K_RETURN,   a(MAIN.doMovePoint)
  dta k_INSERT,   a(MAIN.doAddPoint)
  dta k_DELETE,   a(MAIN.doDeletePoint)
  dta k_ESC,      a(MAIN.doExitEditMode)
end;

procedure movePointShortKeys(); Assembler; Keep;
asm
  dta K_UP,       a(MAIN.doMovePoint)
  dta K_DOWN,     a(MAIN.doMovePoint)
  dta K_LEFT,     a(MAIN.doMovePoint)
  dta K_RIGHT,    a(MAIN.doMovePoint)
end;

procedure showPathBegin();
begin
  updateGameScreen();

  putImage(@_RT_EDGES,18,0,2,48);
  n:=addZone(36,0,4,12,@doSetBeginEdge);   // Top
  assignHintToZone(n,'SET BEGIN TO TOP EDGE');
  n:=addZoneV(@doSetBeginEdge);            // Left
  assignHintToZone(n,'SET BEGIN TO LEFT EDGE');
  n:=addZoneV(@doSetBeginEdge);            // Right
  assignHintToZone(n,'SET BEGIN TO RIGHT EDGE');
  n:=addZoneV(@doSetBeginEdge);            // Bottom
  assignHintToZone(n,'SET BEGIN TO BOTTOM EDGE');

  HPOSP[3]:=180; updateStartPoint();
end;

procedure showPathInner();
begin
  updateGameScreen();

  putImage(@_LT_TOOLS,18,0,2,48);
  n:=addZone(36,0,4,12,@doMovePoint);
  assignHintToZone(n,'MOVE PATH POINT');
  n:=addZoneV(@doAddPoint);            // Add point
  assignHintToZone(n,'ADD NEW PATH POINT');
  n:=addZoneV(@doDeletePoint);         // Delete point
  assignHintToZone(n,'DELETE PATH POINT');
  n:=addZoneV(@nullProc);              // More...
  assignHintToZone(n,'MORE TOOLS...');

  _curPx:=-128; _curPy:=-128;
  _curDx:=-128; _curDy:=-128;

  addZoneN(ZONE_TIMELINE,8,65,32,7,@doSetTimeLinePos);
  zoneInOutCall[ZONE_TIMELINE]:=@inTimeLine;

  addZoneN(ZONE_VIEWPORT,XVIEWPORT div 4,0,26,48,@doMovePoint); // @enterEditPath);
  zoneInOutCall[ZONE_VIEWPORT]:=@inGameScreen;

  HPOSM[0]:=48+XTIMELINE+curTLPos-TLShift;

  HPOSP[3]:=180; curTool:=0; updateCurrentTool();

  registerShortcutKeys(@innerPathShortKeys,SK_INNER_COUNT);
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

  name:=getTrailName(curTrailID);
  setStatus('EDIT:'); putTextC(6,69,name);

  SetScreenWidth(20);
  putImage(@_LT_SECTIONS,0,0,3,48);

  n:=addZone(0,0,3,12,@doBack2Selector);      // Back to trail selector
  assignHintToZone(n,'BACK TO TRAIL SELECTOR');
  n:=addZoneH(@nullProc);                      // Options
  assignHintToZone(n,'EDITOR CONFIGURATION');
  n:=addZone(0,12,6,12,@doSwitchPathSection); // Begin trail
  assignHintToZone(n,'BEGIN TRAIL SETUP');
  n:=addZoneV(@doSwitchPathSection);           // Path
  assignHintToZone(n,'PATH EDIT');
  n:=addZoneV(@doSwitchPathSection);           // Finish trail
  assignHintToZone(n,'FINISH TRAIL SETUP');

  zoneCall[3]:=@doTLPrevEvent;
  zoneCall[6]:=@doTLNextEvent;

  curSection:=-1;
  if getTrailSize(curTrailID)=0 then
    selZone:=11   // show Begin path
  else
    selZone:=12;  // show trail path

  TLShift:=0; curTLPos:=0;
  doSwitchPathSection();
 SK_curTool:=nil;SKcount_curTool:=0;
  registerShortcutKeys(@globalShortKeys,SK_GLOBAL_COUNT);
End;
