#!/bin/bash
cd assets
./make.sh
cd ..

cd data
./make.sh
cd ..

cd core
mpc buildlib core.pas -data:0400 -define:DISABLEIOCBCOPY
cd ..

echo "- Copy CORE.LIB to main BIN..."
rm bin/core.lib
cp core/bin/core.lib bin/core.lib

mpc buildlib about.pas -define:DISABLEIOCBCOPY
mpc buildlib disk.pas -define:DISABLEIOCBCOPY
mpc buildlib pathed.pas -define:DISABLEIOCBCOPY
mpc buildlib scened.pas -define:DISABLEIOCBCOPY

mpc buildcom SIED.pas -code:8000 -define:DISABLEIOCBCOPY