; this is a windows batch file to compile hi.c

set SDCCINC=c:\usr\local\sdcc\device\include
set SDCCLIBS=c:\usr\local\sdcc\device\lib\small

sdcc -I $(SDCCINC) hi.c sdcclib.lib -L $(SDCCINC)
