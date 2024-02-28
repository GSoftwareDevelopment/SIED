#!/bin/bash

rm *.bin
xxd -r -p asc2int.hex asc2int.bin
