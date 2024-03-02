#!/bin/bash
echo "- Preparing assets..."
rm bin/*.bin

# prepare editor assets
xxd -r -p hex/controls.hex bin/controls.bin
xxd -r -p hex/icons.hex bin/icons.bin
xxd -r -p hex/icon-card.hex bin/icon-card.bin
xxd -r -p hex/icon-path.hex bin/icon-path.bin

xxd -r -p hex/font.hex bin/font.bin
xxd -r -p hex/font-col.hex bin/font-col.bin
mads fonts.a65 -o:bin/fonts.bin | grep "ERROR:"
mads assets.a65 -o:bin/assets.bin | grep "ERROR:"