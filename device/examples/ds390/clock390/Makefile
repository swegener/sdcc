CC = sdcc

MFLAGS = -mds390 --model-flat24 --stack-10bit
LFLAGS = --xram-loc 0x100080 --code-loc 0x10000 -Wl-r

all: clock390.hex

clean:
	rm -f *~ \#* *.asm *.cdb *.rel *.hex *.ihx *.lst *.map *.rst *.sym *.lnk

clock390.hex: clock390.ihx
	packihx clock390.ihx >clock390.hex

clock390.ihx: clock390.rel lcd.rel
	$(CC) $(MFLAGS) $(LFLAGS) clock390.rel lcd.rel

%.rel: %.c
	$(CC) -c $(MFLAGS) $<
