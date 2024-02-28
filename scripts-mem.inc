zp_tm           = $14;
zp_nFrmTm       = $12;

zp_curGroup     = $1c;   //
zp_curPwrSprite = $1d;   //
zp_curTrlSprite = $1e;   //

zp_Ax1          = $20;   //  \
zp_Ay1          = $21;   //  |
zp_Ax2          = $22;   //  |
zp_Ay2          = $23;   //  |
zp_Bx1          = $24;   //  |
zp_By1          = $25;   //  | Bounding Box
zp_Bx2          = $26;   //  | Registers
zp_By2          = $27;   //  |
zp__x1          = $28;   //  |
zp__y1          = $29;   //  |
zp__x2          = $2a;   //  |
zp__y2          = $2b;   //  /

zp_nxtTLEvent   = $2c;   // word

zp_scr          = $2e;   // pointer

zp_bcdScore     = $70;   // longint - 6 digits of score
zp_bcdLevel     = $74;   // 2 digits: Level & Stage
zp_playerLives  = $75;   //
zp_bcdMissiles  = $76;   // 2 digits of Missiles

zp_F1Pwr        = $77;   // base weapon: Laser Power
zp_F1Spd        = $78;   // base weapon: Laser Speed
zp_F2Pwr        = $79;   // alternative weapon: Power

zp_keycode      = $7a;
zp_joyDirection = $7b;
zp_joyFire1     = $7c;    // +1 _joyFire1
zp_joyFire2     = $7e;    // +1 _joyFire2

zp_activePage   = $d4;
zp_i            = $d5;
zp_j            = $d6;
zp_trailAdr     = $d7;   // pointer
zp_DYVal        = $d9;
zp_DXVal        = $da;

zp_spriten      = $e0;
zp_spritex      = $e1;
zp_spritey      = $e2;
zp_spritei      = $e3;

zp_gameStat     = $e4;
zp_TLAdr        = $e5;   // pointer
zp_timers       = $e7;   // timer enabled
zp_timer0       = $e8;
zp_timer1       = $e9;
zp_timer2       = $ea;
zp_timer3       = $eb;
zp_timer4       = $ec;
zp_timer5       = $ed;
zp_curTimeLine  = $ee;   // word

//

// mem_sprite_x        = SPR_ENGINE_ADDR+0*(MAX_SPRITE+1);
// mem_sprite_y        = SPR_ENGINE_ADDR+1*(MAX_SPRITE+1);
// mem_sprite_type     = SPR_ENGINE_ADDR+2*(MAX_SPRITE+1);
// mem_sprite_power    = SPR_ENGINE_ADDR+3*(MAX_SPRITE+1);
// mem_sprite_trailLo  = SPR_ENGINE_ADDR+4*(MAX_SPRITE+1);
// mem_sprite_trailHi  = SPR_ENGINE_ADDR+5*(MAX_SPRITE+1);
// mem_sprite_group    = SPR_ENGINE_ADDR+6*(MAX_SPRITE+1);
// mem_sprite_fire     = SPR_ENGINE_ADDR+7*(MAX_SPRITE+1);

// mem_group           = SPR_ENGINE_ADDR+13*(MAX_SPRITE+1)+(MAX_GROUP+1)*0;
// mem_reward          = SPR_ENGINE_ADDR+14*(MAX_SPRITE+1)+(MAX_GROUP+1)*1;