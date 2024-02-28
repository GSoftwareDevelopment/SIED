var
  GPRIOR:Byte absolute $26F;
  SDMACTL:Byte absolute $22F;
  [volatile] PMCTL:Byte absolute $D01D;
  [volatile] PMBASE:Byte absolute $D407;
  [volatile] HPOSP:array[0..3] of Byte absolute $D000;
  [volatile] SIZEP:array[0..3] of Byte absolute $D008;
  PCOL:Array[0..3] of Byte absolute 704;
