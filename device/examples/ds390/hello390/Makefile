hello390.hex: hello390.ihx
	packihx hello390.ihx >hello390.hex

hello390.ihx: hello390.c
	sdcc -mds390 --model-flat24 --stack-10bit \
		--xram-loc 0x100080 --code-loc 0x10000 \
		-Wl-r hello390.c

clean:
	rm -f *~ \#* *.asm *.cdb *.rel *.hex *.ihx *.lst *.map *.rst *.sym *.lnk
