#!/bin/bash

echo "- Preparing data..."

mkdir -p bin
rm bin/*.bin
xxd -r -p hex/asc2int.hex bin/asc2int.bin
xxd -r -p hex/scan2asc.hex bin/scan2asc.bin

mads display-list.a65 -o:bin/display-list.bin | grep "ERROR:"
