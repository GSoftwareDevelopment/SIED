MAX_SPRITE          = 15;                   // max 16 enemys on screen
MAX_GROUP           = 7;                    // max 8 groups

SCREEN_LINES_ADDR   = $0600;                // table of screen lines addresses
DLIST_ADDR          = $1F00;                // Display List definition
ASC2INT_ADDR        = $2000; // $1F00;                // ATASCII to INTERNAL convertion table
SCAN2ASC_ADDR       = $2080; // $1F80;                // Keyboard SCANCODES to ATASCII convertion table
PMG_ADDR            = $2000;                // PMG Base address
SCREEN_ADDR         = $2400;                // Game screen base address
EDITOR_ADDR         = $2780;                // Editor screen base address

FONTS_ADDR          = $3000;                // (1329) font tables, font 4x5 1bpp two half, font 4x5 2bpp

ASSETS_ADDR         = FONTS_ADDR    +1329;  // assets base address
CONTROLS_ADDR       = ASSETS_ADDR   +   0;  // time controls
ICONS_ADDR          = ASSETS_ADDR   +  24;  // module icons
ICARD_ADDR          = ASSETS_ADDR   +  42;  // selected module background
IPATH_ADDR          = ASSETS_ADDR   +  44;  // tray icons for path module
CARROW_ADDR         = ASSETS_ADDR   +  46;
CWAIT_ADDR          = ASSETS_ADDR   +  48;

MODULE_ADDR         = $4000;                // module base address

CORE_LIB            = $D800;                // core library base address
