# Simple Makefile for dhrystone and sdcc

#CC = /home/michaelh/projects/sdcc/bin/sdcc
# -DNOENUM is here to make the results more predictable
#CFLAGS = -mgbz80 -v --dumpall
CC = lcc-gb
CFLAGS = -int16 -DSDCC=1 -v
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

dhry.o: dhry.c
	/usr/lib/SDK/gbz80-gb/2.1.0/bin/cpp -DINT_16_BITS -D__STDC__=1 -DZ80 -DGB -DGAMEBOY -D__LCC__ -DSDCC=1 -DREG= -DNOSTRUCTASSIGN -DNOENUM -DBROKEN_SDCC=0 -DHZ=100 -I/usr/lib/sdcc/include dhry.c dhry.i
	/usr/lib/SDK/gbz80-gb/2.1.0/bin/rcc -target=gbz80/int16 -optimize -v dhry.i dhry.asm
	$(AS) -plosff dhry.o dhry.asm