const
  MBUT_NONE   = 0;
  MBUT_LEFT   = 1;

var
    mRefresh:Boolean  absolute $5c;
        _omy:Byte     absolute $53;
         _mx:Byte     absolute $54;
         _my:Byte     absolute $55;
          mx:Shortint absolute $56;
          my:Shortint absolute $57;
         mdx:Shortint absolute $5a;
         mdy:Shortint absolute $5b;
       stick:Byte     absolute $278;
     mbutton:Byte     absolute $4a;
    ombutton:Byte     absolute $4b;

  CURSORSPR :Pointer  absolute $5e;
  mpivotX   :shortint absolute $60;
  mpivotY   :shortint absolute $61;
