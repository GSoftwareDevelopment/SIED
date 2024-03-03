#!/bin/bash

validChanges() {
  FN="${2}"

  if [ "$FN" != "" ]; then
    HASHFN=".hash-${FN::-3}"
  else
    HASHFN=".hash"
  fi
	if [ -f "$HASHFN" ]; then
    OLDHASH=`cat $HASHFN`
  else
    OLDHASH=''
  fi
# generate new hash
  if [ "$FN" == "" ]; then
    FN="*.*"
  fi
  sha256sum <(sha256sum $FN) > $HASHFN
  NEWHASH=`cat $HASHFN`
  if [ "$NEWHASH" = "$OLDHASH" ]; then
    return 0
  else
    return 1
  fi
}

cd assets
validChanges .
ASSETS=$?
[[ $ASSETS = 1 ]] && ./make.sh;

cd ..

cd data
validChanges .
DATA=$?
[[ $DATA = 1 ]] && ./make.sh

cd ..

cd core
if [[ $ASSETS = 1 || $DATA = 1 ]]; then
  CORE=1
else
  validChanges .
  CORE=$?
fi
[[ $CORE = 1 ]] && ../mpc buildlib core.pas -data:0400 -define:DISABLEIOCBCOPY
cd ..

validChanges . "about*.*"
[[ $? = 1 || $CORE = 1 ]] && ./mpc buildlib about.pas -define:DISABLEIOCBCOPY

validChanges . "disk*.*"
[[ $? = 1 || $CORE = 1 ]] && ./mpc buildlib disk.pas -define:DISABLEIOCBCOPY

validChanges . "pathed*.*"
[[ $? = 1 || $CORE = 1 ]] && ./mpc buildlib pathed.pas -define:DISABLEIOCBCOPY

validChanges . "scened*.*"
[[ $? = 1 || $CORE = 1 ]] && ./mpc buildlib scened.pas -define:DISABLEIOCBCOPY


./mpc buildcom SIED.pas -code:8000 -define:DISABLEIOCBCOPY