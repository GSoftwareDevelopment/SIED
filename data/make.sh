#!/bin/bash

rm bin/*.bin
xxd -r -p hex/asc2int.hex bin/asc2int.bin
xxd -r -p hex/scan2asc.hex bin/scan2asc.bin