PRJ=$1

if [ -z "$PRJ" ]; then
   PRJ=test_mdu517
fi

if [ -f ${PRJ}.ihx ]; then
    ../s51 -t517 -C conf.cmd -Z6666 -S in=/dev/null,out=${PRJ}.out -G ${PRJ}.ihx
    cat ${PRJ}.out
fi
