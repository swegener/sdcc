# Simple Makefile for dhrystone and sdcc

CC = /home/michaelh/projects/sdcc/bin/sdcc
# -DNOENUM is here to make the results more predictable
CFLAGS = -mgbz80 -v --dumpall
CFLAGS += -DREG= -DNOSTRUCTASSIGN -DNOENUM -DBROKEN_SDCC=0 -DHZ=100
LIBDIR = /home/michaelh/projects/sdcc/device/lib/gbz80/
LD = link-gb
AS = as-gb

OBJ = dhry.o

all: dhry

dhry: $(OBJ)
	$(LD) -n -- -z -m -k$(LIBDIR) -lz80.lib \
	-b_CODE=0x200 dhry.gb $(LIBDIR)crt0.o $(OBJ)
#	cat dhry.ihx | ../../makebin/makebin > dhry.rom

native:
	gcc -g -O2 -DREG= -DNOSTRUCTASSIGN -DNOENUM -o dhry dhry.c

.c.o:
	$(CC) $(CFLAGS) $<

dhry.c:

clean:
	rm -r *~ dhry
