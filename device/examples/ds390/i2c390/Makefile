CC = sdcc

MFLAGS = -mds390 --model-flat24 --stack-10bit
LFLAGS = --xram-loc 0x100080 --code-loc 0x10000 -Wl-r

all: i2c390.hex

clean:
	rm -f *~ \#* *.asm *.cdb *.rel *.hex *.ihx *.lst *.map *.rst *.sym *.lnk

OBJECTS = i2c390.rel i2clole.rel ds1621.rel pcf8591.rel

i2c390.hex: i2c390.ihx
	packihx i2c390.ihx >i2c390.hex

i2c390.ihx: $(OBJECTS)
	$(CC) $(MFLAGS) $(LFLAGS) $(OBJECTS)

%.rel: %.c
	$(CC) -c $(MFLAGS) -I../float390 $<
