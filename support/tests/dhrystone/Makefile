# Simple Makefile for dhrystone and sdcc
TOPDIR = ../../..

PROC = z80

CC = $(TOPDIR)/bin/sdcc

# -DNOENUM is here to make the results more predictable
CFLAGS += -DREG= -DNOSTRUCTASSIGN -DNOENUM -V -m$(PROC)

OBJ = dhry.o

all: dhry.ihx

dhry.ihx: dhry.c
	$(CC) $(CFLAGS) dhry.c

dhry.c: dhry.h

dhry.bin: dhry.ihx
	cat $< | ../../makebin/makebin > $@

native:
	gcc -g -O2 -DREG= -DNOSTRUCTASSIGN -DNOENUM -o dhry dhry.c

clean:
	rm -f *~ dhry *.o *.gb *.ihx *.rel
