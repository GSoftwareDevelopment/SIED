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

procedure registerToolShortKeys(SK:Pointer);
begin
  SK_curTool:=SK;
  registerShortcutKeys(SK);
end;

procedure unregisterToolShortKeys();
begin
  if (SK_curTool<>nil) then
  begin
    unregisterShortcutKeys(SK_curTool);
    SK_curTool:=nil;
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
  dta $FF
end;

procedure beginPathShortKeys(); Assembler;
asm
  dta k_UP,       a(MAIN.doSetBeginEdge)
  dta k_LEFT,     a(MAIN.doSetBeginEdge)
  dta k_RIGHT,    a(MAIN.doSetBeginEdge)
  dta k_DOWN,     a(MAIN.doSetBeginEdge)
  dta $FF
end;

procedure innerPathShortKeys(); Assembler;
asm
  dta k_COMMA,    a(MAIN.doTLPrevEvent)
  dta k_DOT,      a(MAIN.doTLNextEvent)
  dta K_RETURN,   a(MAIN.doMovePoint)
  dta k_INSERT,   a(MAIN.doAddPoint)
  dta k_DELETE,   a(MAIN.doDeletePoint)
  dta k_ESC,      a(MAIN.doExitEditMode)
  dta $FF
end;

procedure editModeShortKeys(); Assembler; Keep;
asm
  dta k_ESC,      a(MAIN.doExitEditMode)
  dta K_UP,       a(MAIN.doMovePoint)
  dta K_DOWN,     a(MAIN.doMovePoint)
  dta K_LEFT,     a(MAIN.doMovePoint)
  dta K_RIGHT,    a(MAIN.doMovePoint)
  dta $FF
end;

procedure showPathBegin();
begin
  fillchar(YSCR[56+48],280,0);
  setControls(%000000);

  putImage(@_RT_EDGES,18,0,2,48);
  n:=addZone(36,0,4,12,@doSetBeginEdge); assignHintToZone(n,'SET BEGIN TO TOP EDGE');
  n:=addZoneV(@doSetBeginEdge);          assignHintToZone(n,'SET BEGIN TO LEFT EDGE');
  n:=addZoneV(@doSetBeginEdge);          assignHintToZone(n,'SET BEGIN TO RIGHT EDGE');
  n:=addZoneV(@doSetBeginEdge);          assignHintToZone(n,'SET BEGIN TO BOTTOM EDGE');

  updateStartPoint();
  updateTimeLineTime();
  registerShortcutKeys(@beginPathShortKeys);
end;

procedure showPathInner();
begin
  setControls(%001001);

  putImage(@_RT_TOOLS,18,0,2,48);
  n:=addZone(36,0,4,12,@doMovePoint);  assignHintToZone(n,'MOVE PATH POINT');
  n:=addZoneV(@doAddPoint);            assignHintToZone(n,'ADD NEW PATH POINT');
  n:=addZoneV(@doDeletePoint);         assignHintToZone(n,'DELETE PATH POINT');
  n:=addZoneV(@nullProc);              assignHintToZone(n,'MORE TOOLS...');

  _curPx:=-128; _curPy:=-128;
  _curDx:=-128; _curDy:=-128;
  addZoneN(ZONE_TIMELINE,(XTIMELINE div 4),62,TIMELINE_SIZEB,7,@doSetTimeLinePos);
  zoneInOutCall[ZONE_TIMELINE]:=@inTimeLine;

  addZoneN(ZONE_VIEWPORT,XVIEWPORT div 4,0,26,48,@doMovePoint);
  zoneInOutCall[ZONE_VIEWPORT]:=@inGameScreen;

  updateTimeLineTime();
  updateCurrentTool(-1);

  registerShortcutKeys(@innerPathShortKeys);
end;

procedure showPathFinish();
begin
  setControls(%000000);
  fillchar(YSCR[56+48],280,0);

  putImage(@_RT_FINISH,18,0,2,48);
  n:=addZone(36,0,4,12,@nullProc);  assignHintToZone(n,'SET LOOP POINT');
  n:=addZoneV(@nullProc);           assignHintToZone(n,'STOP OBJECT');
  n:=addZoneV(@nullProc);           assignHintToZone(n,'KILL OBJECT');
  n:=addZoneV(@nullProc);           assignHintToZone(n,'JUMP TO DIFFERENT TRAIL');

  updateTimeLineTime();
  updateFinishMode(0);
end;

//
//
//

procedure showTrailEditor();
begin
  curSection:=-1;
  TLShift:=0; curTLPos:=0;
  SK_curTool:=nil;
  cursorInViewport:=false;

  for n:=0 to 2 do zoneActive[n]:=false;
  fillchar(YSCR[56+48],960,0);
  clearWorkarea();
  clearStatus();

  // addZoneN(4,0,52,2,7,@nullProc);        // play
  // addZoneHN(5,@nullProc);                // stop
  addZoneN(3,0,61,2,7,@doTLPrevEvent);   // previous
  addZoneHN(6,@doTLNextEvent);           // next

  setControls(%001001); // only Prev & Next controls
  setControl(-1);

  name:=getTrailName(curTrailID);
  setStatus('TRAIL EDITOR');

  SetScreenWidth(20);
  putImage(@_LT_SECTIONS,0,0,3,48);

  n:=addZone(0,0,3,12,@doBack2Selector); assignHintToZone(n,'BACK TO TRAIL SELECTOR');
  n:=addZoneH(@nullProc); assignHintToZone(n,'EDITOR CONFIGURATION');
  n:=addZone(0,12,6,12,@doSwitchPathSection); assignHintToZone(n,'BEGIN TRAIL SETUP');
  n:=addZoneV(@doSwitchPathSection); assignHintToZone(n,'PATH EDIT');
  n:=addZoneV(@doSwitchPathSection); assignHintToZone(n,'FINISH TRAIL SETUP');

  if getTrailSize(curTrailID)=0 then
    selZone:=11   // show Begin path
  else
    selZone:=12;  // show trail path

  doSwitchPathSection();
End;
