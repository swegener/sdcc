# makefile for Linux

all : blink.ihx

blink.ihx :
	asx8051 -los blink.asm
	aslink -f blink.lnk

clean:
	rm mon.ihx *.lst *.rel *.rst *.sym *.cdb *.map *.ihx

