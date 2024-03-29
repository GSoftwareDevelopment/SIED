FPS                 = 25;                   // Timeline second divisor (for PAL)
BASE_COLOR          = $E0;

MAX_SPRITE          = 15;                   // max 16 enemys on screen
MAX_GROUP           = 7;                    // max 8 groups

ASC2INT_ADDR        = $2000;                // ATASCII to INTERNAL convertion table
SCAN2ASC_ADDR       = $2080;                // Keyboard SCANCODES to ATASCII convertion table
PMG_ADDR            = $2000;                // PMG Base address
SCREEN_ADDR         = $2400;                // Game screen base address
EDITOR_ADDR         = $2780;                // Editor screen base address
STATUS_ADDR         = $2e60;                //

// assets
ASSETS_ADDR         = $3000;                // assets base address
CONTROLS_ADDR       = ASSETS_ADDR   +   0;  // time controls
ICONS_ADDR          = ASSETS_ADDR   +  24;  // module icons
ICARD_ADDR          = ASSETS_ADDR   +  42;  // selected module background

// interface assets
VSCROLL_ADDR        = ASSETS_ADDR   +  44;
ERASEINPUT_ADDR     = ASSETS_ADDR   +  46;
RADIOBUT_ADDR       = ASSETS_ADDR   +  48;
// cursor shapes
CARROW_ADDR         = ASSETS_ADDR   +  50;
CWAIT_ADDR          = ASSETS_ADDR   +  52;
CBEAM_ADDR          = ASSETS_ADDR   +  54;
CCROSS_ADDR         = ASSETS_ADDR   +  56;

RLEBUFFER_ADDR      = $3C00;                // address of RLE Compression buffer
RLEBUFFERSIZE       = 1024;                 // RLE Compression buffer size

HINTBUF_ADDR        = $3C00;                // hint draw buffer
TLBUF_ADDR          = $3F00;                // TimeLine Drawing Buffer

MODULE_ADDR         = $4000;                // module base address

DLIST_ADDR          = $8000;                // Display List definition

MAXPATHDEFINITIONS  = 100;                  // Max trail definitions

CARD_ADDR           = $A000;

PATHLISTV_ADDR      = $C000;                // Table of path vectors ((MAXPATHDEFINITIONS+1)*Pointer)
PATHSTARTEDGE_ADDR  = $C100;                // Table of start point X (MAXPATHDEFINITIONS*Byte)
PATHNAMES_ADDR      = $C200;                // Address of path names table

SCREEN_LINES_ADDR   = $D800;                // table of screen lines addresses

CORE_LIB            = $DA00;                // core library base address

FONTS_ADDR          = $F800;                // (1329) font tables, font 4x5 1bpp two half, font 4x5 2bpp
SCKEYVEC_ADDR       = $FD40;                // shortcur keyboard vectors
ITIMER_ADDR         = $FDC0;                // interface timers
IZONE_ADDR          = $FE00;                // interface work tables
