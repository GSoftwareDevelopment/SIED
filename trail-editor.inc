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

{$I 'modules/trail-editor/graph.inc'}
{$I 'modules/trail-editor/trail-process.inc'}

//
//
//

const
  SK_GLOBAL_COUNT     = 4;
  SK_BEGIN_COUNT      = 4;
  SK_INNER_COUNT      = 6;
  SK_MOVEPOINT_COUNT  = 5;

procedure globalShortKeys(); assembler; Forward; Keep;
procedure beginPathShortKeys(); Assembler; Forward;
procedure innerPathShortKeys(); Assembler; Forward;
procedure editModeShortKeys(); Assembler; Forward; Keep;

function doSwitchPathSection():boolean; Forward;
procedure showPathBegin(); Forward;
procedure showPathInner(); Forward;
procedure showPathFinish(); Forward;
{$I 'trail-editor-controls.inc'}
{$I 'trail-editor-actions.inc'}
{$I 'modules/trail-editor/timeline.inc'}
{$I 'modules/trail-editor/edit.inc'}

procedure globalShortKeys(); assembler; Keep;
asm
  dta k_ESC,      a(MAIN.doBack2Selector)
  dta k_B,        a(MAIN.doSwitchPathSection)
  dta k_I,        a(MAIN.doSwitchPathSection)
  dta k_F,        a(MAIN.doSwitchPathSection)
end;

procedure beginPathShortKeys(); Assembler;
asm
  dta k_UP,       a(MAIN.doSetBeginEdge)
  dta k_LEFT,     a(MAIN.doSetBeginEdge)
  dta k_RIGHT,    a(MAIN.doSetBeginEdge)
  dta k_DOWN,     a(MAIN.doSetBeginEdge)
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

procedure editModeShortKeys(); Assembler; Keep;
asm
  dta k_ESC,      a(MAIN.doExitEditMode)
  dta K_UP,       a(MAIN.doMovePoint)
  dta K_DOWN,     a(MAIN.doMovePoint)
  dta K_LEFT,     a(MAIN.doMovePoint)
  dta K_RIGHT,    a(MAIN.doMovePoint)
end;

procedure showPathBegin();
begin
  fillchar(YSCR[56+48],280,0);
  updateGameScreen();
  zoneActive[3]:=false;
  zoneActive[6]:=false;

  putImage(@_RT_EDGES,18,0,2,48);
  n:=addZone(36,0,4,12,@doSetBeginEdge);   // Top
  assignHintToZone(n,'SET BEGIN TO TOP EDGE');
  n:=addZoneV(@doSetBeginEdge);            // Left
  assignHintToZone(n,'SET BEGIN TO LEFT EDGE');
  n:=addZoneV(@doSetBeginEdge);            // Right
  assignHintToZone(n,'SET BEGIN TO RIGHT EDGE');
  n:=addZoneV(@doSetBeginEdge);            // Bottom
  assignHintToZone(n,'SET BEGIN TO BOTTOM EDGE');

  updateStartPoint();
  registerShortcutKeys(@beginPathShortKeys,SK_BEGIN_COUNT);
end;

procedure showPathInner();
begin
  updateGameScreen();

  putImage(@_RT_TOOLS,18,0,2,48);
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
  zoneActive[3]:=true;
  zoneActive[6]:=true;
  addZoneN(ZONE_TIMELINE,(XTIMELINE div 4),62,TIMELINE_SIZEB,7,@doSetTimeLinePos);
  zoneInOutCall[ZONE_TIMELINE]:=@inTimeLine;

  addZoneN(ZONE_VIEWPORT,XVIEWPORT div 4,0,26,48,@doMovePoint);
  zoneInOutCall[ZONE_VIEWPORT]:=@inGameScreen;

  HPOSM[0]:=48+XTIMELINE+curTLPos-TLShift;

  updateCurrentTool(-1);

  registerShortcutKeys(@innerPathShortKeys,SK_INNER_COUNT);
end;

procedure showPathFinish();
begin
  zoneActive[3]:=false;
  zoneActive[6]:=false;
  fillchar(YSCR[56+48],280,0);

  HPOSP[3]:=180;
  putImage(@_RT_FINISH,18,0,2,48);
  n:=addZone(36,0,4,12,@nullProc);
  assignHintToZone(n,'LOOP');
  n:=addZoneV(@nullProc);
  assignHintToZone(n,'STOP');
  n:=addZoneV(@nullProc);
  assignHintToZone(n,'KILL');
  n:=addZoneV(@nullProc);
  assignHintToZone(n,'JUMP...');
end;

//
//
//

procedure showTrailEditor();
begin
  for n:=0 to 2 do zoneActive[n]:=false;
  fillchar(YSCR[56+48],960,0);
  clearWorkarea();
  clearStatus();

  // addZoneN(4,0,52,2,7,@nullProc);        // play
  // addZoneHN(5,@nullProc);                // stop
  addZoneN(3,0,61,2,7,@doTLPrevEvent);   // previous
  addZoneHN(6,@doTLNextEvent);           // next

  setControls(%001001); // only Prev/Play/Stop/Next controls
  setControl(-1);

  name:=getTrailName(curTrailID);
  setStatus('TRAIL EDITOR');
  // putTextC(0,56,'NAME:')
  // putTextC(6,56,name);

  SetScreenWidth(20);
  putImage(@_LT_SECTIONS,0,0,3,48);

  n:=addZone(0,0,3,12,@doBack2Selector); assignHintToZone(n,'BACK TO TRAIL SELECTOR');
  n:=addZoneH(@nullProc); assignHintToZone(n,'EDITOR CONFIGURATION');
  n:=addZone(0,12,6,12,@doSwitchPathSection); assignHintToZone(n,'BEGIN TRAIL SETUP');
  n:=addZoneV(@doSwitchPathSection); assignHintToZone(n,'PATH EDIT');
  n:=addZoneV(@doSwitchPathSection); assignHintToZone(n,'FINISH TRAIL SETUP');

  curSection:=-1;
  if getTrailSize(curTrailID)=0 then
    selZone:=11   // show Begin path
  else
    selZone:=12;  // show trail path

  TLShift:=0; curTLPos:=0;
  doSwitchPathSection();
  SK_curTool:=nil; SKcount_curTool:=0;
  cursorInViewport:=false;
End;
