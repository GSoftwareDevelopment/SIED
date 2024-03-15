const
  FPS = 25;
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
    // $38,$00,$10,$00,$80,$A0,$80,$00,$01,$05,$01,$00,$00,$10,$00,$38
    // $54,$00,$10,$00,$00,$80,$00,$A0,$00,$80,$01,$00,$05,$00,$01,$00,$00,$10,$00,$54
    $7C,$38,$10,$00,$00,$80,$C0,$E0,$C0,$80,$01,$03,$07,$03,$01,$00,$00,$10,$38,$7C
  ];
  _TLSELECT:Array of byte = [ 3,3, $10,$10,$92,$54,$92,$10,$10 ];

  // _LOOP:Array of byte = [ $A0, $82, $0A, $28 ];
  _LOOP:Array of byte = [ $F5, $D7, $5F, $7D ];

  ZONE_VIEWPORT = 30;
  ZONE_EDGE     = 26;
  XVIEWPORT     = 32;
  XCORNERS:Array[0..3] of byte = ( XVIEWPORT+48, XVIEWPORT, XVIEWPORT+96, XVIEWPORT+48 );
  YCORNERS:Array[0..3] of byte = (   0, 23, 23,  43 );

  _TOP          =  0;
  _MIDDLE       = 24;
  _BOTTOM       = 47;
  _LEFT         = -7;
  _CENTER       = 39;
  _RIGHT        = 94;

  _EDGETOP      = 0;
  _EDGELEFT     = 1;
  _EDGERIGHT    = 2;
  _EDGEBOTTOM   = 3;

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

  XTIMELINE       = 32;
  TIMELINELENGTH  = 128; // 128 pixels= 5s 03f (PAL 1/50)
  ZONE_TIMELINE   = 31;

  PATH_ANIM_TIME = 5;

  ANIM:Array[0..5] of longint = (
  // 765432107654321076543210
    %000111000111000111000111,
    %001110001110001110001110,
    %011100011100011100011100,
    %111000111000111000111000,
    %110001110001110001110001,
    %100011100011100011100011

    // %001100110011001100110011,
    // %011001100110011001100110,
    // %110011001100110011001100,
    // %100110011001100110011001
  );

  PT_CALCLEN          = %0001;
  PT_UPDATETIMELINE   = %0010;
  PT_UPDATEPATH       = %0100;
  PT_KEEPCURSOR       = %1000;

  SP_ONLYPOINT        = 0;
  SP_DESCRIPTION      = 1;