#!/bin/bash
cd assets
./make.sh
cd ..

rm *.bin
xxd -r -p asc2int.hex asc2int.bin
xxd -r -p scan2asc.hex scan2asc.bin

cd core
mpc buildlib core.pas -define:DISABLEIOCBCOPY
cd ..

mpc buildcom SIED.pas -code:8000 -define:DISABLEIOCBCOPY