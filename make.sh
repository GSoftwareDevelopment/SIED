#!/bin/bash
export ERRORLEVEL="error-only"
export KEEPLOG=0
export LOG=1
BUILD=''
BUILDDISK=0

for arg; do
  case $arg in
  all)
    BUILD="all";
    ;;
  disk)
    BUILD="all";
    BUILDDISK=1;
    ;;
  full-log)
    ERRORLEVEL="full-log"
    ;;
  keep-log)
    KEEPLOG=1
    ;;
  *)
    ;;
  esac
done
# echo "${BUILD} ${BUILDDISK} ${ERRORLEVEL}"

validChanges() {
  FN="${1}"

  if [ "$FN" != "" ]; then
    HASHFN=".hash-${FN::-3}"
  else
    HASHFN=".hash"
  fi
  if [ -f "$HASHFN" ]; then
    OLDHASH=`cat $HASHFN`
    rm $HASHFN
  else
    OLDHASH=''
  fi
# generate new hash
  if [ "$FN" == "" ]; then
    FN="*.*"
  fi
  COUNT=$(ls | wc -l)
  if [ $COUNT != 0 ]; then
    sha256sum <(sha256sum $FN) > $HASHFN
    NEWHASH=`cat $HASHFN`
  else
    NWEHASH=''
  fi
  if [ "$BUILD" = "all" ]; then
    return 1
  else
    if [ "$NEWHASH" = "$OLDHASH" ]; then
      return 0
    else
      return 1
    fi
  fi
}

cd asm
validChanges
[[ %? = 1 ]] && BUILD="all"
cd ..

cd assets
validChanges
ASSETS=$?
[[ $ASSETS = 1 ]] && ./make.sh;

cd ..

cd data
validChanges
DATA=$?
[[ $DATA = 1 ]] && ./make.sh

cd ..

cd core
validChanges
CORE=$?
[[ $ASSETS = 1 ]] && CORE=1;
[[ $DATA = 1 ]] && CORE=1;
[[ $CORE = 1 ]] && ../mpc buildlib core.pas -data:D980 -define:DISABLEIOCBCOPY
cd ..

validChanges "about*.*"
[[ $? = 1 || $CORE = 1 ]] && ./mpc buildlib about.pas -define:DISABLEIOCBCOPY

validChanges "disk*.*"
[[ $? = 1 || $CORE = 1 ]] && ./mpc buildlib disk.pas -define:DISABLEIOCBCOPY

validChanges "trail*.*"
[[ $? = 1 || $CORE = 1 ]] && ./mpc buildlib trail.pas -define:DISABLEIOCBCOPY

validChanges "scened*.*"
[[ $? = 1 || $CORE = 1 ]] && ./mpc buildlib scened.pas -define:DISABLEIOCBCOPY

./mpc buildcom SIED.pas -code:8100 -define:DISABLEIOCBCOPY

if [ $BUILDDISK = 1 ]; then
echo "- Build disk image..."
cd tools
cp mydos450.atr sied.atr
./atr sied.atr put ../bin/SIED.com AUTORUN.SYS
./atr sied.atr put ../bin/about.lib ABOUT.LIB
./atr sied.atr put ../bin/disk.lib DISK.LIB
./atr sied.atr put ../bin/trail.lib TRAIL.LIB
./atr sied.atr put ../bin/scened.lib SCENED.LIB
cd ..
fi
