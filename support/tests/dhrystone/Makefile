# Simple Makefile for dhrystone and sdcc

PROC = gbz80
PORT = gb

CC = /home/michaelh/projects/gbdk-support/lcc/lcc
# -DNOENUM is here to make the results more predictable
CFLAGS = -Wl-m -mz80/consolez80 -v --prefix=/home/michaelh/projects/gbdk-current/
CFLAGS += -DREG= -DNOSTRUCTASSIGN -DNOENUM
LIBDIR = /home/michaelh/projects/gbdk-lib/build/
LD = link-$(PROC)
LDFLAGS = -v
AS = as-$(PROC)

OBJ = dhry.o

all: dhry

dhry: dhry.s $(OBJ)
	-$(CC) $(CFLAGS) -o dhry.gb dhry.o
	cat dhry.ihx | ../../makebin/makebin  > dhry.rom

dhry.s: dhry.c
	$(CC) $(CFLAGS) -S -o dhry.s dhry.c

native:
	gcc -g -O2 -DREG= -DNOSTRUCTASSIGN -DNOENUM -o dhry dhry.c

dhry.c:

clean:
	rm -f *~ dhry *.o *.gb
