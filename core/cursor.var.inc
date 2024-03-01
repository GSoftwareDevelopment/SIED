const
  MBUT_NONE   = 0;
  MBUT_LEFT   = 1;

var
    mRefresh:Boolean  absolute $5c;
  mAllowKeys:Boolean  absolute $5d;
         _mx:Byte     absolute $52;
         _my:Byte     absolute $53;
          mx:Shortint absolute $55;
          my:Shortint absolute $54;
         mdx:Shortint absolute $5b;
         mdy:Shortint absolute $5a;
       stick:Byte     absolute $278;
     mbutton:Byte     absolute $4a;
    ombutton:Byte     absolute $4b;

  CURSORSPR :Pointer  absolute $5e;
  mpivotX   :shortint absolute $60;
  mpivotY   :shortint absolute $61;
