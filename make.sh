#!/bin/bash
cd assets
./make.sh
cd ..

rm *.bin
xxd -r -p asc2int.hex asc2int.bin

mpc buildcom SIED.pas -code:8000 -define:DISABLEIOCBCOPY