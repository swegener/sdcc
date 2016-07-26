PRJ=$1

if [ -z "$PRJ" ]; then
   PRJ=t1
fi

../stlcs -C c ${PRJ}.ihx <run.cmd
