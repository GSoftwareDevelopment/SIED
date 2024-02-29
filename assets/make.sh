#!/bin/bash

# rm bin/*.bin

# prepare editor assets
xxd -r -p controls.hex controls.bin
xxd -r -p icons.hex icons.bin
xxd -r -p icon-card.hex icon-card.bin
xxd -r -p icon-path.hex icon-path.bin

xxd -r -p font.hex font.bin
xxd -r -p font-col.hex font-col.bin
mads fonts.a65 -o:fonts.bin