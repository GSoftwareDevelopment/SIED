const
  FPS = 25; // Timeline second divisor (for PAL)
  _LT_SECTIONS :Array of byte = [ // Left Tray icons for sections edit
    $00,$00,$00,$00,$00,$00,$00,$00,$C0,$10,$00,$60,$3F,$82,$20,$7F,$A3,$60,$3F,$A1,$F0,$10,$20,$38,$07,$E0,$14,$08,$00,$08,$00,$00,$00,$00,$00,$00,$00,$00,$00,$0E,$EE,$B0,$0A,$88,$A8,$0C,$CA,$A8,$0A,$8A,$A8,$0E,$EE,$A8,$00,$00,$00,$00,$1C,$00,$00,$3E,$00,$00,$1C,$00,$00,$08,$00,$00,$00,$00,$00,$08,$00,$00,$00,$00,$00,$10,$00,$05,$40,$00,$10,$00,$00,$00,$01,$40,$20,$14,$10,$01,$40,$00,$14,$00,$10,$00,$05,$40,$00,$10,$00,$00,$00,$00,$00,$00,$00,$00,$38,$00,$00,$7C,$00,$00,$38,$00,$00,$10,$00,$00,$00,$00,$1D,$64,$D4,$11,$55,$14,$19,$54,$9C,$11,$54,$54,$11,$55,$94,$00,$00,$00
  ];
  _ICOORD :Array of byte = [    // Background shape for edit coordinates
    $02,$AA,$6A,$AA,$AA,$AA,$AA,$AA,$00,$AA,$6A,$AA,$AA,$AE,$AA,$AA,$00,$A5,$96,$AA,$AA,$BA,$AA,$AA,$00,$2A,$6A,$AA,$AA,$EA,$AA,$AA,$00,$2A,$6A,$AA,$AA,$AA,$AA,$AA,$00,$0A,$AA,$AA,$AA,$AA,$AA,$AA,$AA,$AA,$AA,$AA,$AA,$AA,$AA,$A8,$AA,$56,$AA,$AA,$AE,$AA,$AA,$A0,$AA,$96,$AA,$AA,$BA,$AA,$AA,$A0,$AA,$66,$AA,$AA,$EA,$AA,$AA,$80,$A9,$AA,$AA,$AA,$AA,$AA,$AA,$80,$AA,$AA,$AA,$AA,$AA,$AA,$AA,$00
  ];
  _LT_TOOLS :Array of byte = [  //  Left Tray icons for edit tools
    $00,$00,$01,$80,$03,$C0,$00,$00,$09,$90,$1A,$58,$1A,$58,$09,$90,$00,$00,$03,$C0,$01,$80,$00,$00,$00,$00,$00,$02,$00,$06,$00,$0E,$3E,$1C,$77,$3A,$77,$74,$41,$68,$77,$70,$77,$00,$3E,$00,$00,$00,$00,$00,$00,$06,$00,$0E,$00,$1E,$3E,$3E,$7F,$4E,$7F,$46,$41,$42,$7F,$22,$7F,$1C,$3E,$00,$00,$00,$00,$00,$97,$77,$F5,$54,$D5,$66,$95,$54,$97,$57,$00,$00,$03,$80,$07,$C0,$03,$80,$01,$00,$00,$00
  ];
  _RT_EDGES :aRRAY OF BYTE = [  // Right Tray icons for begin edges
    $00,$00,$7B,$DE,$47,$E2,$43,$C2,$01,$80,$00,$00,$00,$00,$00,$00,$40,$02,$40,$02,$70,$0E,$00,$00,$00,$00,$70,$0E,$40,$02,$20,$02,$70,$00,$78,$00,$78,$00,$70,$00,$20,$02,$40,$02,$70,$0E,$00,$00,$00,$00,$70,$0E,$40,$02,$40,$04,$00,$0E,$00,$1E,$00,$1E,$00,$0E,$40,$04,$40,$02,$70,$0E,$00,$00,$00,$00,$70,$0E,$40,$02,$40,$02,$00,$00,$00,$00,$00,$00,$01,$80,$43,$C2,$47,$E2,$7B,$DE,$00,$00
  ];

// Tray mask for selected item
  _TRAYLEFT :Array of byte = [
    $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,
    $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,
    $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,
    $80,$80,$80,$80,$80,$80,$80,$80,$80,$80,$80,$80,
    $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,
    $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,
    $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
  ];
  _TRAYRIGHT :Array of byte = [
    $3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,
    $3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,
    $3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,
    $01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,
    $3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,
    $3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,
    $3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F
  ];
  _EDGES:Array of byte = [  // Edge symbols
    $7C,$38,$10,$00,$00,  // Top
    $80,$C0,$E0,$C0,$80,  // Left
    $01,$03,$07,$03,$01,  // Right
    $00,$00,$10,$38,$7C   // Bottom
  ];

  _IPOINTARROW:Array of byte = [
    $F0,$E0,$F0,$B8,$10,  // Left-Up
    $1E,$0E,$1E,$3A,$10,  // Right-Up
    $10,$B8,$F0,$E0,$F0,  // Left-Down
    $10,$3A,$1E,$0E,$1E   // Right-Down
  ];

  // cursors shapes
  _TLSELECT:Array of byte = [ // Timeline select cursor
    3,3, $10,$10,$92,$54,$92,$10,$10
  ];
  _STOP:Array of byte = [     // Viewport stop cursor
    3,3, $38,$44,$A2,$92,$8A,$44,$38
  ];

// TimeLine '//' filler (Loop)
  _TLLOOP:Array of byte = [ $AA, $F5, $D7, $5F, $AA ];

  ZONE_VIEWPORT = 30;           // Viewport zone ID
  XVIEWPORT     = 32;           // Viewport X Offset

// Edge IDs
  EDGE_TOP      = 0;
  EDGE_LEFT     = 1;
  EDGE_RIGHT    = 2;
  EDGE_BOTTOM   = 3;

// Edge symbols positions in Viewport orientation
  _XEDGE:Array[0..3] of byte = ( 48,  0, 96, 48 );
  _YEDGE:Array[0..3] of byte = (  4, 27, 27, 47 );

// Edge constans
  _TOP          =  0;
  _MIDDLE       = 25; // Vertical middle
  _BOTTOM       = 47;
  _LEFT         = -7;
  _CENTER       = 43; // Horizontal middle
  _RIGHT        = 94;

// Edge coords in Viewport orientation
  XEDGE:Array of shortint = [ _CENTER, _LEFT,   _RIGHT,  _CENTER ];
  YEDGE:Array of shortint = [ _TOP,    _MIDDLE, _MIDDLE, _BOTTOM ];

// Drawing helpers
  PBIT:Array[0..7] of byte = ( 128,64,32,16,8,4,2,1 );
  P4MASK:Array[0..3] of byte = ( %00111111, %11001111, %11110011, %11111100 );
  P4COLOR:Array[0..2,0..3] of byte = (
    ( %01000000, %00010000, %00000100, %00000001 ),
    ( %10000000, %00100000, %00001000, %00000010 ),
    ( %11000000, %00110000, %00001100, %00000011 )
  );

  XTIMELINE       = 32;                     // X offset of TimeLine
  ZONE_TIMELINE   = 31;                     // TimeLine zone ID
  TIMELINELENGTH  = 128;                    // 128 pixels= 5s 03f (PAL 1/50)
  TIMELINE_SIZEB  = TIMELINELENGTH div 4;   // timeline draw buffer width in bytes

  PATH_ANIM_REFRESH = 5;                    // frame delay for path refresh

  PATHANIM:Array[0..5] of longint = (       // Loop of path Line shape
  // 765432107654321076543210
    %000111000111000111000111,
    %001110001110001110001110,
    %011100011100011100011100,
    %111000111000111000111000,
    %110001110001110001110001,
    %100011100011100011100011
  );

// Process Trail config
  PT_CALCLEN          = %0001;    // Calculate real length of trail
  PT_UPDATETIMELINE   = %0010;    // Update timeline draw
  PT_UPDATEPATH       = %0100;    // Update path draw

// Edit Tools
  TOOL_MOVEPOINT      = 1;
  TOOL_ADDPOINT       = 2;
  TOOL_DELPOINT       = 3;
